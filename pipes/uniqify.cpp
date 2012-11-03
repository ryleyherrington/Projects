/*
* Original Author: Ryley Herrington
* File: uniqify.cpp
* Created: 2012 January 28, by herringr 
* Last Modified: 2012 January 31, 21:25 by herringr
* 
* This file contains functions to:
*     create N child "sort" processes each connected by two pipes
*     read stdin and distribute the contents to the "sort" sub-processes
*     read the N sorted outputs from the sub-processes
*     create a "suppressor" process to eliminate duplicates
*     perform a merge sort on these N streams and write to the "suppressor" 
*/

// #include <iostream>
// #include <cstring>
#include <vector>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

/*
 *  Utility functions to handle system/library call failures
 */
static void checked_close(int fd)          { if (close(fd) < 0)       { perror("close");  exit(-1); } }
static void checked_fclose(FILE* stream)   { if (fclose(stream) != 0) { perror("fclose"); exit(-1); } }
static void checked_pipe(int fd[2])        { if (pipe(fd) < 0)        { perror("pipe");   exit(-1); } }
static void checked_dup2(int fd1, int fd2) { if (dup2(fd1, fd2) < 0)  { perror("dup2");   exit(-1); } }

/*
 *  Utility function to convert upper case characters to lower case within a string
 */
static char* downcase_str(char* word) {
    for (int i=0; word[i] != '\0'; i++)
        word[i] = tolower(word[i]);
    return word;
}

#define MAX_LINE_LENGTH 10240
#define MAX_WORD_LENGTH   200

// Used to distinguish the ends of our pipes 
static const int READ_END=0;
static const int WRITE_END=1;

/*
 *  This bundles information on communicating with each "sort" subprocess
 */
class connection_t {
  public:
    pid_t   child_id;
    int     parent_write[2]; // parent will write to this fd
    int     parent_read[2];  // parent will read from this fd
    connection_t() { 
        parent_write[0] = -1;
        parent_write[1] = -1;
        parent_read[0] = -1;
        parent_read[0] = -1;
        child_id = (pid_t) -1;
    }
};


/*
 *  distribute_words()
 *
 *  This method will read from an input stream and round-robin
 *  distribute them to N output streams.  
 *
 *  In this program, these output streams will be tied to external
 *  sorting processes, but that is not important (or even visible)
 *  to the implementation of this function.
 */
static void distribute_words(FILE* input_file, vector<FILE*> streams) {

    // Set up string of delimiters 
    char delimiters[256], *dp = delimiters;
    for (int i=1; i<256; i++) {    // runs through all ascii characters*/
        if (!isalpha(i))
            *dp++ = i;             // add to the delimeter string */
    }
    *dp = '\0';                    // terminate the string

    int count = streams.size();
    char buff[MAX_LINE_LENGTH], *p;
    char *word;                   // each distinct word of input 
    int curr_file = 0;
    while (fgets(buff, sizeof(buff), input_file) != NULL) {
        p = buff;
        while((word = strsep(&p, delimiters))!= NULL) {

            // skip if there were ONLY non-alpha chars until end-of-line
            if (!isalpha(word[0]))
                continue;

            // write word to an output stream (after converting to lower case)
            fprintf(streams[curr_file], "%s\n", downcase_str(word)); 

            // select next stream "round robin"
            curr_file = (curr_file + 1) % count;
        }
    }

    // close all the streams (and their underlying fds) 
    for (int c=0; c<count; c++)
        checked_fclose(streams[c]);
    checked_fclose(input_file);
}


/*
 * Read new word from stream into provided buffer
 *
 * On success, fill the provided buffer and return 1
 *
 * When an input stream is empty:
 *    close the FILE*
 *    free() the word buffer
 *    return 0 
 */
static int get_word(FILE*f, char*word) {
    if (fgets(word, MAX_WORD_LENGTH -1, f) == NULL) {
        checked_fclose(f);
        free(word);
        return 0;
    }
    return 1;
}

/*
 *   merge_words() -  merge sort
 *
 *   For "W" words distributed across "F" files,
 *   we perform "F-1" comparisons * "W" words to be written
 *
 *   This might be improved by keeping the "current" words in
 *   a more sophisticated structure (ie. a sorted heap), but
 *   realistically, that would likely only matter for large N.
 *   If I understand the idea of mergesort, it's really for
 *   sorting data sets larger than memory, so the I/O time
 *   of getting the data probably dominates the comparison
 *   time in this merge.
 */
static void merge_words(FILE* output_file, vector<FILE*> input_streams) {

    /*
     *  Create one word buffer for each input stream
     *
     *  This buffer will always hold the current word from the stream and
     *  will be deallocated and set to NULL to signify when the stream is empty
     */
    int count = input_streams.size();
    vector<char*> words(count);
    for (int i=0; i<count; i++) {
        words[i] = (char*)malloc(MAX_WORD_LENGTH );
        if (words[i] == NULL)
            perror("malloc"), exit(-1);

        // initialize by reading the first word from each stream
        if (get_word(input_streams[i], words[i]) == 0)
            words[i] = NULL;
    }

    // Exit this processing loop when we detect that all streams are empty
    while (true) {

        /*
         * Find the first non-NULL word buffer
         * This will be compared to all others to find the smallest
         * If there are no non-NULL word buffers, then all streams are empty, and we're done
         */
        int smallest = -1;     // assume there is no valid word buffer
        for (int i=0; i<count; i++) {
            if (words[i] != NULL) {
                smallest = i;  // we now have a condidate for the "smallest" word
                break;
            }
        }

        // If no word found, we're done, so return
        if (smallest == -1)
            return;

        // Compare rest of words to find smallest of current set
        for (int i=smallest+1; i<count; i++) {
            if ((words[i] != NULL) && (strcmp(words[i], words[smallest]) < 0)) {
                smallest = i;
            }
        }

        // Write out the smallest token, and re-fill from its stream
        fprintf(output_file, "%s", words[smallest]);
        if (get_word(input_streams[smallest], words[smallest]) == 0)
            words[smallest] = NULL;
    }
    checked_fclose(output_file);
}

/*
 *  Create a vector of streams from the child fds 
 */
static vector<FILE*> createWriteStreams(vector<connection_t> conn) {
    int count = conn.size();
    vector<FILE*> streams_to_write(count);
    for (int c=0; c<count; c++ )
        streams_to_write[c] = fdopen(conn[c].parent_write[WRITE_END], "w");
    return streams_to_write;  
}

static vector<FILE*> createReadStreams(vector<connection_t> conn) {
    int count = conn.size();
    vector<FILE*> streams_to_read(count);
    for (int c=0; c<count; c++ ) 
        streams_to_read[c] = fdopen(conn[c].parent_read[READ_END], "r"); // open parent pipe RE
    return streams_to_read;
}

/*
 * 
 */
static void wait_for_children(vector<connection_t> conn) {
    int count = conn.size();
    for (int c=0; c<count; c++) {
        int status = 0;
        pid_t child = waitpid(conn[c].child_id, &status, 0);
        if ((int)child == -1 && (errno != EINTR)) {
            perror("waitpid");
            exit(-1);
        }
        if (WEXITSTATUS(status) != 0)
            fprintf(stderr, "Child process %ld exited with non-zero status %d\n", (long)child, WEXITSTATUS(status));
    }
}

/*
 *  parser
 *
 *  This multi-process sort is composed of: 1 parser, N sorters, 1 suppressor
 *
 *  This "parser" has the following jobs:
 *      - Create the sorting processes and connect each with an input and output pipe
 *      - Create the suppressor and connect a single pipe to its input
 *      - Read stdin and distribute distinct words to each sorting process (round-robin)
 *      - Mergesort the results from all of the sorting processes
 *      - Pass the merged data to the suppressor to eliminate duplicates
 */
int main(int argc, char *argv[]) {

    // We won't allow a file as an argument; always use "stdin"
    FILE* input_file = stdin;

    // optional argument determines how many sort processes to use
    int count = (argc > 1) ? atoi(argv[1]) : 1;


    // Set up the suppressor process with a single "write" pipe
    int pipe_to_suppressor[2];
    checked_pipe(pipe_to_suppressor);
    pid_t sup_id; 
    switch ( (sup_id=fork()) ) { 
        case -1: 
            perror("forking"); 
            exit(-1);

        case 0:     // Child process
            checked_close(pipe_to_suppressor[WRITE_END]);
            checked_dup2(pipe_to_suppressor[READ_END], 0);  // closes previous fd 0
            execlp("suppressor", "suppressor", (char *)NULL);
            perror("suppressor");   // will never be called if exec works 
            exit(-1);

        default:
            checked_close(pipe_to_suppressor[READ_END]);
            break;
    }    
    //FILE *output_file = stdout;  // for debugging
    FILE *output_file = fdopen(pipe_to_suppressor[WRITE_END], "w");


    // Create "sort" sub-processes and connect each with input and output pipes
    vector<connection_t> conn(count); 
    for (int c=0; c<count; c++) {
    
        checked_pipe(conn[c].parent_write);
        checked_pipe(conn[c].parent_read);
     
        switch( (conn[c].child_id=fork()) ) {
        case -1:
            perror("fork didn't work properly.");
            exit(-1);
    
        case 0:
            // In child: read from parent write, write to parent read
            checked_close(conn[c].parent_write[WRITE_END]);
            checked_close(conn[c].parent_read[READ_END]);

            checked_dup2(conn[c].parent_read[WRITE_END], 1);  // closes previous fd 1
            checked_dup2(conn[c].parent_write[READ_END], 0);  // closes previous fd 0

            execlp("sort", "sort", (char *)NULL);
            perror("sort");   // will never be called if exec works 
            exit(-1);              // also won't be called if exec works 
            
        default: 
            // In parent: write to parent write, read from parent read 
            checked_close(conn[c].parent_read[WRITE_END]);
            checked_close(conn[c].parent_write[READ_END]);
            break;
        }
    }

    vector<FILE*> streams_to_write = createWriteStreams(conn);
    distribute_words(input_file, streams_to_write);               // writes words to child sort processes

    vector<FILE*> streams_to_read = createReadStreams(conn);
    merge_words(output_file, streams_to_read);                    // mergesorts what the kids write

    wait_for_children(conn);

    // Waiting for suppressor child process   
    int sup_status = 0;
    pid_t child = waitpid(sup_id, &sup_status, WNOHANG);
    // pid_t child = waitpid(sup_id, &sup_status, 0);
    if ((int)child == -1) {
        perror("wait");
        exit(-1);
    }
    if (WEXITSTATUS(sup_status) != 0)
        fprintf(stderr, "Suppressor sub-process %ld exited with non-zero status %d\n", (long)child, WEXITSTATUS(sup_status));
    return 0;
}
