
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

// Global state variables
static int kill_command_port = 21047;

/* 
 * handle_signals
 *
 * Currently this does not attempt anything
 * resembling a "graceful" shutdown.
 */
void handle_signals(int sig)
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
void register_signal_handlers()
{
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = handle_signals;
    act.sa_flags   = SA_RESTART;
    sigemptyset (&act.sa_mask);
    sigaction(SIGHUP,  &act, 0);
    sigaction(SIGINT,  &act, 0);
    sigaction(SIGQUIT, &act, 0);
}

/*
 * error_and_exit: Report error message and exit process
 */
void error_and_exit(const char *msg)
{
    perror(msg);
    exit(0);
}

/*
 * activity_monitor : kill compute client if inactive for too long
 */
static time_t last_command_time = 0;

void monitor_update()
{
    last_command_time = time(0);
}

void *monitor_activity(void*p)
{
    while (1) {
        sleep(1800); // sleep for 30 minutes
        time_t curr_time = time(0);
        if ((curr_time - last_command_time) > 1800) {
            fprintf(stderr, "compute client detected > 30 minutes of inactivity ... exiting\n");
            exit(0);
        }
    }
}

/*
 *  watch_for_kill_command: set up and monitor a port, waiting for a "K" command
 *
 *  This thread will set up a server socket and block, waiting for a
 *  KILL command (the letter "K").  
 *
 *  Rather than exiting by simply calling "exit", the instructions say for
 *  the process to send itself a INTR signal, which will be caught and the
 *  handler will call exit().
 */
void* watch_for_kill_command(void* p)
{
    // Create the KILL socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
       error_and_exit("ERROR: Unable to open socket");
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // Look for an open port and bind to it
    bool found_port = false;
    for (int i=0; i<100; i++) {
        serv_addr.sin_port = htons(kill_command_port);
        if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) >= 0) {
            found_port = true;
            break;
        }
        kill_command_port++;
    }
    if (!found_port) {
        error_and_exit("ERROR: Unable to find open port for KILL command\n");
        exit(-1);
    }

    // Listen for a KILL command
    listen(sockfd,5);

    // Accept the connection to create a new "fd"
    struct sockaddr_in kill_sock_addr;
    socklen_t kill_sock_len = sizeof(kill_sock_addr);
    int newsockfd = accept(sockfd, (struct sockaddr *)&kill_sock_addr, &kill_sock_len);
    if (newsockfd < 0)
         error_and_exit("ERROR: Accept failed\n");

    // Read the KILL command
    char buffer[100];
    memset(buffer, 0, sizeof(buffer));
    int n = read(newsockfd, buffer, sizeof(buffer)-1);
    if (n < 0) 
        error_and_exit("ERROR: Read failure from KILL socket");
    fprintf(stderr, "watch_for_kill_command: detected 'K' command\n");    // debug

    // Acknowledge the KILL command
    n = write(newsockfd, "Q",1);
    if (n < 0)
        error_and_exit("ERROR: Unable to write to the KILL socket");

    close(newsockfd);
    close(sockfd);

    fprintf(stderr, "watch_for_kill_command: wrote the 'Q'\n");           // debug
    fprintf(stderr, "watch_for_kill_command: sending SIGINT\n");          // debug
    kill(getpid(), SIGINT);

    return 0;
}

/*
 * connect_to_server: connect to the advertised port on the "manage" server
 */
int connect_to_server(char* server_name, int port_num)
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
int
send_command_to_server(char *server_name, int port_num, const char *command)
{
    fprintf(stderr, "send_command_to_server: %s", command);     // debug
    int server_fd = connect_to_server(server_name, port_num);
    if (write(server_fd, command, strlen(command)) < 0)
        error_and_exit("Compute client could not write to manager server");
    return server_fd;
}

/*
 * read_range_from_server: Read the low/high range for next sequence to analyze
 */
void read_range_from_server(int server_fd, long *low, long *high)
{
    // Read the low value and high value of the range of numbers to check
    char *p, reply_buffer[1024] = { 0 };
    if (read(server_fd, reply_buffer, sizeof(reply_buffer)) < 0)
        error_and_exit("Compute client could not read from manager server");
    fprintf(stderr, "read_range_from_server: '%s'\n", reply_buffer);                  // debug

    float float_low, float_high;
    sscanf(reply_buffer, "%f %f", &float_low, &float_high);
    *low  = float_low;
    *high = float_high;
    fprintf(stderr, "read_range_from_server: low = %ld high = %ld\n", *low, *high);   // debug
}

/*
 * compute_perfect_numbers: Use brute force analysis of all numbers within given range
 */
std::vector<long> compute_perfect_numbers(long low, long high)
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
    if (perf_nums.size() != 0)                                          // debug
        fprintf(stderr, "compute_perfect_numbers: "                     // debug
                "found %ld perfect numbers\n", (long)perf_nums.size()); // debug

    time_t end_calc = time(0);                                                                // debug
    fprintf(stderr, "compute_perfect_numbers: %ld seconds\n", (long)(end_calc - start_calc)); // debug
    return perf_nums;
}

/*
 * compute_OPS : compute operations per second for this machine
 *
 * We could measure the system we're on using IOPS or FLOPS, 
 * but it seems more practical to measure it based on the 
 * operation that we'll be performing.  Thus, our measured
 * "operation" will be a "value checked as a factor" (which
 * is probably dominated by the time for a "%" operation).
 *
 * Since we're insisting on the 'brute force" approach, when
 * testing if N is perfect, we at least conceptually test each
 * of the (approximately) N values below it to see if they 
 * are a factor.  It doesn't matter that we might explicitly
 * only test half of them, we're still determining if any of 
 * the (approximately) N values less than N are factors.
 *
 * Since it takes N operations to determine if N is perfect,
 * then it will take N*(N+1)/2 operations to search all of
 * the numbers up to and including N.  To simplify things,
 * let's approximate that as N^2/2.
 *
 * This means that to search a range from L to H, we would
 * perform approximately H^2/2 - L^2/2 operations.  If we
 * perform this calculation and measure it, we can divide by
 * the time to produce an "operations per second" metric.
 *
 * The server can then use this metric to determine the
 * appropriate size of range to pass to this calculation
 * client (in order to keep the calculation in the 15 second
 * range).
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
 * So, when the server is assigning a new range, it should use
 * the smallest unassigned value for the low bound and use the
 * previous formula to determine the upper bound.
 */
double
compute_OPS()
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
 *   -- The port number for registering the KILL command
 *   -- The value of the computed operations per second
 * 
 * For example, it might look like: 
 *
 *      I mymachine 12345 21047 1000000.0
 *
 * Because all data is in string form, byte ordering is not an issue.
 */
std::string
construct_I_command()
{
    struct utsname uname_buff;
    if (uname(&uname_buff) < 0)
        error_and_exit("Compute client could not find host name");
    char command_buffer[1024];
    sprintf(command_buffer, "I %s %ld %d %f\n", 
            uname_buff.nodename, (long)getpid(), kill_command_port, compute_OPS());
    std::string command(command_buffer);
    fprintf(stderr, "construct_I_command: %s", command.c_str());  // debug
    return command;
}

/*
 * construct_P_command : Create the command to report computed perfect numbers
 *   -- The command character "P"
 *   -- Each of the perfect numbers found within the given range
 *
 * For example, it might look like:
 *
 *      P 6 28 496
 */
std::string
construct_P_command(std::vector<long> number_vec)
{
    std::string command("P ");

    char numbuff[100];
    for (int i=0; i<number_vec.size(); i++) {
        sprintf(numbuff, "%ld ", number_vec[i]);
        command += numbuff;
    }
    command += "\n";

    fprintf(stderr, "construct_P_command: %s", command.c_str());  // debug
    return command;
}

int main(int argc, char *argv[])
{
    char *server_name = argc>1 ? argv[1]       : (char*)"localhost";
    long server_port  = argc>2 ? atol(argv[2]) : 2245;

    // Prepare process to handle SIGINT, SIGHUP, SIGQUIT
    register_signal_handlers();

    // spin off a thread to listen for a KILL command
    pthread_t kill_thr;
    pthread_create(&kill_thr, 0, watch_for_kill_command, 0);

    // spin off a thread to act as a "deadman" monitor
    pthread_t deadman_thr;
    monitor_update();
    pthread_create(&deadman_thr, 0, monitor_activity, (void*)0);

    // Register this compute client with an "I" command
    std::string command = construct_I_command();
    int server_fd = send_command_to_server(server_name, server_port, command.c_str());

    // Continue computing perfect numbers until told to quit
    while (1) {

        // Read the low value and high value of the range of numbers to check
        long low, high;
        read_range_from_server(server_fd, &low, &high);
        close(server_fd);

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
