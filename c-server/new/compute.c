
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <vector> 
#include <string>
#include <sys/utsname.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>

/*
 * error_and_exit: Report error message and exit process
 */
static void error_and_exit(const char *msg)
{
    perror(msg);
    exit(0);
}

/* 
 * handle_signals
 *
 * Currently this does not attempt anything
 * resembling a "graceful" shutdown.
 */
static void handle_signals(int sig)
{
    switch (sig) {
       case SIGHUP : fprintf(stderr, "Received SIGHUP  ... exiting\n");        break;
       case SIGINT : fprintf(stderr, "Received SIGINT  ... exiting\n");        break;
       case SIGQUIT: fprintf(stderr, "Received SIGQUIT ... exiting\n");        break;
       default     : fprintf(stderr, "Received signal %d ... exiting\n", sig); break;
    }
    exit(0);
}

/*
 * register_signal_handlers: register handlers for HUP, INTR, QUIT 
 */
static void register_signal_handlers()
{
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = handle_signals;
    act.sa_flags   = SA_RESTART;
    sigemptyset (&act.sa_mask);
    if (sigaction(SIGHUP,  &act, 0) < 0)
        error_and_exit("ERROR: Unable to register signal handler");
    if (sigaction(SIGINT,  &act, 0) < 0)
        error_and_exit("ERROR: Unable to register signal handler");
    if (sigaction(SIGQUIT, &act, 0) < 0)
        error_and_exit("ERROR: Unable to register signal handler");
}

/*
 * monitor_activity : kill compute client if time limits exceeded
 *
 * Kill process if 
 *     - inactive for 10 minutes (means "manage" is hung)
 *     - active for 6 hours (unlikely for this class assignment)
 */
static time_t last_command_time = 0;

static void monitor_update()
{
    last_command_time = time(0);
}

static void *monitor_activity(void*p)
{
    time_t invocation_time = time(0);
    last_command_time = time(0);
    while (1) {
        sleep(10*60); // check every 10 minutes
        time_t curr_time = time(0);
        if (((curr_time - last_command_time) > 10*60) ||      // 10 minutes inactive
            ((curr_time - invocation_time)   > 6*60*60)) {    // 6 hours max execution
            // Could arguably send "K" command to "manage.py"
            fprintf(stderr, "compute client exceeded time limit ... exiting\n");
            exit(0);
        }
    }
}

/*
 * get_local_hostname : return the hostname of the current machine
 */
static char *get_local_hostname()
{
    struct utsname uname_buff;
    if (uname(&uname_buff) < 0)
        error_and_exit("Compute client could not find host name");
    return strdup(uname_buff.nodename);
}

/*
 *  watch_for_kill_command: watch for the "K" command on the server socket
 *
 *  This thread will block on the socket passed in (which was set up
 *  by the "I" command) waiting for a "K" command from the server.
 *  It will respond with a "Q" command.
 *
 *  Rather than exiting by simply calling "exit", the instructions say for
 *  the process to send itself a INTR signal, which will be caught and the
 *  handler will call exit().
 */
static void* watch_for_kill_command(void* p)
{
    // Create the KILL socket
    int sockfd = (intptr_t)p;

    // Read the KILL command
    char buffer[100];
    memset(buffer, 0, sizeof(buffer));
    int n = read(sockfd, buffer, sizeof(buffer)-1);
    if (n < 0) 
        error_and_exit("ERROR: Read failure from KILL socket");
    fprintf(stderr, "watch_for_kill_command: detected 'K' command\n");    // debug

    // Acknowledge the KILL command
    n = write(sockfd, "Q",1);
    if (n < 0)
        error_and_exit("ERROR: Unable to write to the KILL socket");

    if (close(sockfd) < 0)
        error_and_exit("ERROR: close() failed");

    fprintf(stderr, "watch_for_kill_command: wrote the 'Q'\n");           // debug
    fprintf(stderr, "watch_for_kill_command: sending SIGINT\n");          // debug
    kill(getpid(), SIGINT);

    // Should never reach here
    exit(-1);

    return 0;
}

/*
 * connect_to_server: connect to the advertised port on the "manage" server
 */
static int connect_to_server(char* server_name, int port_num)
{
    // look up the server we will connect to
    struct hostent *server = gethostbyname(server_name);
    if (server == NULL) {
        fprintf(stderr,"ERROR: Could not find server '%s' ... exiting\n", server_name);
        exit(0);
    }

    // Create the socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error_and_exit("ERROR: Unable to open socket");

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    serv_addr.sin_port = htons(port_num);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error_and_exit("ERROR: Unable to connect to server");

    return sockfd;
}

/*
 * send_command_to_server : write the provided command string to the "manage" server
 */
static int send_command_to_server(char *server_name, int port_num, const char *command)
{
    //fprintf(stderr, "send_command_to_server: server=%s port=%d command=%s", server_name, port_num, command);     // debug
    int server_fd = connect_to_server(server_name, port_num);
    if (write(server_fd, command, strlen(command)) < 0)
        error_and_exit("Compute client could not write to manager server");
    return server_fd;
}

/*
 * read_range_from_server: Read the low/high range for next sequence to analyze
 */
static void read_range_from_server(int server_fd, long *low, long *high)
{
    // Read the low value and high value of the range of numbers to check
    char *p, reply_buffer[1024] = { 0 };
    if (read(server_fd, reply_buffer, sizeof(reply_buffer)) < 0)
        error_and_exit("Compute client could not read from manager server");
    //fprintf(stderr, "read_range_from_server: '%s'\n", reply_buffer);                  // debug

    float float_low, float_high;
    sscanf(reply_buffer, "%f %f", &float_low, &float_high);
    *low  = (long)float_low;
    *high = (long)float_high;
    //fprintf(stderr, "read_range_from_server: low = %ld high = %ld\n", *low, *high);   // debug
}

/*
 * compute_perfect_numbers: Use brute force analysis of all numbers within given range
 */
static std::vector<long> compute_perfect_numbers(long low, long high)
{
    time_t start_calc = time(0);                                                    // debug
    fprintf(stderr, "compute_perfect_numbers: low = %ld high = %ld\n", low, high);  // debug
    std::vector<long> perf_nums;
    for (long candidate = low; candidate<high; candidate++) {
        long sum = 0;
        for (long i=1; i<=candidate/2; i++) {
            if ((candidate % i) == 0) {
                sum += i;
            }
        }
        if (sum == candidate)
            perf_nums.push_back(candidate);
    }

    //if (perf_nums.size() != 0)                                          // debug
      //  fprintf(stderr, "compute_perfect_numbers: "                     // debug
       //         "found %ld perfect numbers\n", (long)perf_nums.size()); // debug

    time_t end_calc = time(0);                                                                // debug
    fprintf(stderr, "compute_perfect_numbers: took  %ld seconds\n", (long)(end_calc - start_calc)); // debug
    return perf_nums;
}

/*
 * compute_OPS : compute operations per second for this machine
 *
 * We could measure the system we're on using IOPS or FLOPS, but it 
 * seems more practical to measure it based on the operation that we'll 
 * be performing.  Thus, our measured "operation" will be a "value 
 * checked as a factor" (which is probably dominated by the time for 
 * a "%" operation).
 *
 * Since we're insisting on the 'brute force" approach, when testing if
 * N is perfect, we at least conceptually test each of the (approximately)
 * N values below it to see if they are a factor.  It doesn't matter that
 * we might explicitly only test half of them, we're still determining if
 * any of the (approximately) N values less than N are factors.
 *
 * Since it takes N operations to determine if N is perfect, then it
 * will take N*(N+1)/2 operations to search all of the numbers up to 
 * and including N.  To simplify things, let's approximate that as N^2/2.
 *
 * This means that to search a range from L to H, we would perform
 * approximately H^2/2 - L^2/2 operations.  If we perform this calculation
 * and measure it, we can divide by the time to produce an "operations
 * per second" metric.
 *
 * The server can then use this metric to determine the appropriate size
 * of range to pass to this calculation client (in order to keep the
 * calculation in the 15 second range).
 *
 * Given:
 *    OPS = operations/second (provided by "compute" client)
 *    L   = low bound (lowest unsearch value)
 *
 * Find:
 *    H   = high bound of range to give to "compute" client
 *
 * Solve for "H":
 *    (H^2 - L^2) / 2 = ( 15 * OPS)
 *    H^2 - L^2 = 2 ( 15 * OPS)
 *    H^2 = L^2 + 2 * (15 * OPS)
 *    H = sqrt(L^2 + 2 * (15 * OPS))
 *
 * So, when the server is assigning a new range, it should use the 
 * smallest unassigned value for the low bound and use the previous 
 * formula to determine the upper bound.
 */
static double compute_OPS()
{
    long delta_seconds = 1;
    long delta = 5000;
    long lower = 1;
    do {
        // ramp up the upper bound to find at least a 5 second calculation
        // This could be done much more quickly if we used a finer resolution timer
        delta *= 2;
        time_t start = time(0);
        (void)compute_perfect_numbers(lower, lower+delta);
        time_t end = time(0);
        delta_seconds = ((long)(end - start));
        fprintf(stderr, "compute_OPS: range of %ld .. %ld took %ld seconds\n",  // debug
                lower, lower+delta, delta_seconds);                             // debug
    } while (delta_seconds < 5);

    // compute operations per second : (delta^2/2) / time
    double OPS = ((double)delta * (double)delta) / (2.0 * (double)delta_seconds);

    // return operations per second
    return OPS;
}

/*
 * construct_I_command : Create the initialization command for registering a new compute client
 * 
 * This command will be composed of:
 *   -- The command character "I"
 *   -- The name of the system running the compute client
 *   -- The process id of the compute client
 *   -- The value of the computed operations per second
 * 
 * For example, it might look like: 
 *
 *      I mymachine 12345 1000000.0
 *
 * Because all data is in string form, byte ordering is not an issue.
 */
static std::string construct_I_command()
{
    char *nodename = get_local_hostname();
    char command_buffer[1024];
    sprintf(command_buffer, "I %s %ld %f\n", 
            nodename, (long)getpid(), compute_OPS());
    free(nodename);
    std::string command(command_buffer);
    //fprintf(stderr, "construct_I_command: %s", command.c_str());  // debug
    return command;
}

/*
 * construct_P_command : Create the command to report computed perfect numbers
 *
 * This command will be composed of:
 *   -- The command character "P"
 *   -- Each of the perfect numbers found within the given range
 *
 * For example, it might look like:
 *
 *      P 6 28 496
 */
static std::string construct_P_command(std::vector<long> number_vec)
{
    std::string command("P ");

    char numbuff[100];
    for (int i=0; i<number_vec.size(); i++) {
        sprintf(numbuff, "%ld ", number_vec[i]);
        command += numbuff;
    }
    // command += "\n";

    fprintf(stderr, "construct_P_command: %s\n", command.c_str());  // debug
    return command;
}

int main(int argc, char *argv[])
{
    char *server_name = argc>1 ? argv[1]       :(char*) "os-class.engr.oregonstate.edu";
    long server_port  = argc>2 ? atol(argv[2]) : 2245;

    // Prepare process to handle SIGINT, SIGHUP, SIGQUIT
    register_signal_handlers();

    // spin off a thread to act as a "deadman" monitor
    pthread_t deadman_thr;
    pthread_create(&deadman_thr, 0, monitor_activity, (void*)0);

    // Register this compute client with an "I" command
    std::string command = construct_I_command();
    int server_fd = send_command_to_server(server_name, server_port, command.c_str());
    bool is_initial_fd = true;

    // Continue computing perfect numbers until told to quit
    while (1) {

        // Read the low value and high value of the range of numbers to check
        long low, high;
        read_range_from_server(server_fd, &low, &high);
        if (is_initial_fd) {
            // spin off a thread to listen for a KILL command using the fd from the "I" command
            pthread_t kill_thr;
            pthread_create(&kill_thr, 0, watch_for_kill_command, (void*)server_fd);
            is_initial_fd = false;
        }
        else {
            close(server_fd);
        }

        // Update "deadman" monitor to register that we're still active
        monitor_update();

        // compute the perfect numbers in the range we were told
        // This should take approximately 15 seconds.
        std::vector<long> number_vec = compute_perfect_numbers(low, high);

        // Send any discovered perfect numbers to server and ask for new range
        command = construct_P_command(number_vec);
        server_fd = send_command_to_server(server_name, server_port, command.c_str());
    }
    return 0;
}
