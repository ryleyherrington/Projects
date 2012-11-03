/*
* Original Author: Ryley Herrington
* File: copyprog.c
* Created: 2012 January 19, by herringr 
* Last Modified: 2012 January 22, 18:05 by herringr
* 
* This file contains functions related to copying files with a buffer size.
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>

void err_and_exit(char *errmsg) {
	fprintf(stderr," Error: %s\n", errmsg);
	exit(-1);
}

int main(int argc, char *argv[]) {

	if(argc != 4 || strcmp(argv[1], "--help") == 0) { 
	    err_and_exit("Usage: copyprog source_file destination_file bytesize ");
	}

	char *src = argv[1]; /* source file*/ 
	char *dest = argv[2]; /* destination file*/
	size_t blksize = atoi(argv[3]); /*block size to copy with*/

	int src_fd = open(argv[1], O_RDONLY); /*open source file as read only*/
	if (src_fd == -1) {
		err_and_exit("Can't open source file");
	}

        if (access(argv[2], F_OK) == 0) {
		printf("File '%s' exists, overwrite? (y/n)\n", argv[2]); /*if the file exists, asks user to confirm writing over*/
		char answer = fgetc(stdin); /*get answer from user */
		if (toupper(answer) != 'Y') {
			err_and_exit("Destination file already exists... exiting\n");
		}
		printf("... overwriting\n");
        }

        int flags = O_RDWR | O_CREAT; 
        int permissions = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
	int dest_fd = open(argv[2], flags, permissions); /*open destination file, getting ready to write it out */
	if (dest_fd == -1) {

		err_and_exit("Can't create destination file");
	}
	char *buff = (char *)malloc(blksize); /*sizeof char=1*/
	if(buff == NULL) {
		err_and_exit("Did not malloc properly");	
	}
	size_t r_count;
	while ( (r_count=read(src_fd, buff, blksize)) != 0) { /*getting the amount read in, and checking if there's more to read */
		if (r_count == -1) {
			err_and_exit("Read failure");
		}
		size_t w_count = write(dest_fd, buff, r_count);	
		if (w_count != r_count) {
			err_and_exit("Write failure");
		}
	}
	free(buff); /*free the malloc'd buffer*/
	if (close(dest_fd) != 0) {
		err_and_exit("Unable to close destination file");
	}
	if (close(src_fd) != 0) {
		err_and_exit("Unable to close source file");
	}
        return 0;
}
