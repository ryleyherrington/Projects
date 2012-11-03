#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char filename[1024];
    size_t     filesize;   
}toc_t;


int 
main(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++) { 			/*starting at 1 b/c $0=prog name*/
        /*for -t*/
        if (strcmp(argv[i], "-t") == 0) {
            int num_entries;

            FILE *afile = fopen(argv[i+1],"r");		/*open file for read purposes "w" would be write*/
            if (afile==NULL)
                perror("fopen of archive failed"), exit(-1); /* this is one command, braces are better*/

           fread(&num_entries,sizeof(num_entries),1,afile);
           toc_t *toc = (toc_t *) malloc(sizeof(toc_t) * num_entries);
           fread(toc,sizeof(toc_t),num_entries,afile);
           
           for(int j=0;j<num_entries;j++){
               printf("%20s %lu\n",toc[i].filename,toc[i].filesize);
         } 
       } /*for -c*/
       else if (strcmp(argv[i], "-c") == 0) {
           int num_entries = argc-2;
	   toc_t *toc = (toc_t *) malloc(sizeof(toc_t) * num_entries);
           
           for(int j=0;j<num_entries;j++){
               char *filename = argv[i+2+j];
               strcpy(toc[j].filename,filename);
	       struct stat sb;  
               //put perror here
               stat(filename, &sb); // if returns -1 put error message
	       toc[j].filesize = sb.st_size;
           } 
           FILE *afile = fopen(argv[i+1],"w");		
           fwrite(&num_entries,sizeof(num_entries),1,afile);
           fwrite(toc,sizeof(toc_t),num_entries,afile);
	   
	   char *buff = 0; 
           for(int j=0;j<num_entries;j++){
               FILE * f= fopen(toc[j].filename,"r");
 	       buff = realloc(buff,toc[j].filesize);
	       /*while (file not empty) {copy one MB per iteration in a loop*/
	       fread(buff,toc[j].filesize,1,f);
	       fwrite(buff,toc[j].filesize,1,afile);
           }

       } /*for -x */
       else if (strcmp(argv[i], "-x") == 0) { 

       }

}




}
