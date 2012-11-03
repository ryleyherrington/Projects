#include <stdio.h>
#include <string.h>

#define LINESIZE  1024

int main(int argc, char* argv[]) {

    //fprintf(stderr, "Entering suppressor ...\n");  

    char linebuffer1[LINESIZE], linebuffer2[LINESIZE];
    linebuffer2[0] = '\n';
    linebuffer2[1] = '\0';

    /* 2 buffers to compare if same word */
    char *lastbuff = linebuffer2;
    char *currbuff = linebuffer1;
    while (fgets(currbuff, LINESIZE, stdin) != 0) {

        //fprintf(stderr, "Suppressor read: %s", currbuff);  

        /*compares the strings in the two buffers*/
        if (strncmp(currbuff, lastbuff, LINESIZE) != 0) {
            fprintf(stdout, "%s", currbuff);
        }

        // swaps buffers
        char *temp = lastbuff;
        lastbuff = currbuff;
        currbuff = temp;
    }

    //fprintf(stderr, "Exiting suppressor ...\n");  
    return 0;
}
