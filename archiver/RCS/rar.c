
/*
 *  rar - Ryley's archiver
 *
 *  This archiver is modelled after the Unix "ar" or "tar" archivers, though
 *  it has fewer options and only handles simple files (no diretories).
 *
 *  The format of the archive will be:
 *
 *  .-------------------------------.
 *  | Number of entires in archive  |
 *  |-------------------------------|
 *  | Filename #1                   |
 *  | Filesize #1                   |
 *  |-------------------------------|
 *  | Filename #2                   |
 *  | Filesize #2                   |
 *  |-------------------------------|
 *  | ...                           |
 *  |-------------------------------|
 *  | Filename N                    |
 *  | Filesize N                    |
 *  |-------------------------------|
 *  | Contents of file #1           |
 *  |-------------------------------|
 *  | Contents of file #2           |
 *  |-------------------------------|
 *  | ...                           |
 *  |-------------------------------|
 *  | Contents of file N            |
 *  `-------------------------------'
 */

#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

/*
 *  A more sophisticated tool would use variable length strings
 */
#define MAX_FILE_NAME 1024
typedef struct {
    char file_name[MAX_FILE_NAME];
    size_t file_size;
} toc_t;

typedef struct {
    int    num_entries;
    toc_t *toc;
} header_t;

/*
 *  In a large program, we might want to handle errors more 
 *  gracefully, but in a small utility, it's sufficient to
 *  report the problem and then bail.
 */
void
error_and_exit(char *message)
{
    fprintf(stderr, "Error: %s\n", message);
    exit(-1);
}

/*
 * read_header - read in the header of an archive file
 */
header_t read_header(FILE* archive_file)
{
    int num_entries;

    /* read number of entries from the archive header */
    if (fread(&num_entries, sizeof(num_entries), 1, archive_file) != 1)
        error_and_exit("unable to read archive size");

    toc_t* toc = (toc_t *) malloc(sizeof(toc_t) * num_entries);
    if (toc == NULL)
        error_and_exit("malloc failed");

    /* read the entire table of contents */
    if (fread(toc, sizeof(toc_t), num_entries, archive_file) != num_entries)
        error_and_exit("read of archive file failed");

    /* set up the structure to be returned */
    header_t header;
    header.num_entries = num_entries;
    header.toc = toc;
    return header;
}

/*
 *  list_TOC - dump the table of contents for an archive
 *
 *  archive_name : name of archive file to inspect
 */
void
list_TOC(char *archive_name)
{
    /* open archive file for read, "w" would be for write */
    FILE *archive_file = (strcmp(archive_name, "-")==0) ? stdin : fopen(archive_name, "r");
    
    if (archive_file == NULL)
        error_and_exit("fopen failed reading archive file - exiting");

    /* read in the table of contents */
    header_t header = read_header(archive_file);

    /* for the "-t" option, print out the table of contents */
    for (int j = 0; j < header.num_entries; j++) {
        printf("%20s %lu\n", header.toc[j].file_name, header.toc[j].file_size);
    }

    fclose(archive_file);
}

/*
 *  create_archive - create an archive from a list of files
 *
 *  archive_name : name of archive file to create
 *  filelist     : NULL-terminated array of files to place into archive
 *  num_entries  : count of entries in filelist
 */
void
create_archive(char *archive_name, char *filelist[], int num_entries)
{
    /* allocate space for the in-memory table-of-contents */
    toc_t *toc = (toc_t *) malloc(sizeof(toc_t) * num_entries);
    if (toc == NULL)
        error_and_exit("malloc failed");

    /* loop through the list of files and fill in the table-of-contents */
    for (int j = 0; j < num_entries; j++) {
        char *file_name = filelist[j];

        struct stat sb;
        if (stat(file_name, &sb) == -1) {
            fprintf(stderr, "Unable to stat file '%s' -- exiting\n", file_name);
            exit(-1);
        }

        /* Make sure this is a regular file (not a directory, socket, etc) */
        if (!S_ISREG(sb.st_mode)) {                           /* see "man s 2 stat"*/
            fprintf(stderr, "Unable to archive file '%s' -- not a regular file\n", file_name);
            exit(-1);
        }

        /* Save the file_name and file size in the table of contents */
        strncpy(toc[j].file_name, file_name, MAX_FILE_NAME);   /* avoid over-writing buffer */
        toc[j].file_size = sb.st_size;
    }

    /* write out the archive header (which is num_entries + TOC)*/
    FILE *archive_file = (strcmp(archive_name, "-")==0) ? stdout : fopen(archive_name, "w");
    if (fwrite(&num_entries, sizeof(num_entries), 1, archive_file) != 1)
        error_and_exit("Unable to write number of entries to archive file");
    if (fwrite(toc, sizeof(toc_t), num_entries, archive_file) != num_entries)
        error_and_exit("Unable to write archive file");

    /* loop (again) through the list of files, concatenating each into the archive */
    char *buff = 0;
    for (int j = 0; j < num_entries; j++) {
        fprintf(stdout, "%s\n", toc[j].file_name);

        FILE *f = fopen(toc[j].file_name, "r");
        if (f == NULL)
            error_and_exit("Unable to read file -- exiting");

        /* As a quick solution, read or write entire file in one operation */
        if ((buff = realloc(buff, toc[j].file_size)) == NULL)
            error_and_exit("realloc failed");
        fread(buff, toc[j].file_size, 1, f); /*put loop here if you don't want to suck up memory*/
        fwrite(buff, toc[j].file_size, 1, archive_file);
    }
    fclose(archive_file);
}

/*
 *  find_file_offset - Determine seek offset of specified file
 *
 *  file_name   : name of file to search for
 *  num_entries : number of entries in the table of contents
 *  TOC         : table of contents for this archive
 *  file_size   : reference parameter to return size of file being searched for
 *
 *  RETURNS     : offset of file location within archive or -1 on error
 */
off_t
find_file_offset(char *file_name, int num_entries, toc_t * TOC, size_t * file_size)
{
    off_t offset = 0;

    /* First, skip past the archive header (num_entries and TOC) */
    offset += sizeof(num_entries) + num_entries * sizeof(toc_t);

    /* Skip past files until the specified entry is found */
    for (int j = 0; j < num_entries; j++) {
        if (strcmp(file_name, TOC[j].file_name) == 0) {
            *file_size = TOC[j].file_size;
            return offset;
        }
        else {
            offset += TOC[j].file_size;
        }
    }
    
    /* If we couldn't find the file, report the problem */
    fprintf(stderr, "Unable to find file '%s' in archive - exiting\n",
            file_name);
    *file_size = 0;
    return -1;  /* signifies an error */
}

void
extract_file(char *filename, FILE *archive_file, header_t header)
{
    printf("%s\n", filename); 

    /* for each file to extract, call find file offset and seek to that point  */
    size_t size = 0;
    off_t offset = find_file_offset(filename, header.num_entries, header.toc, &size);

    fseeko(archive_file, offset, SEEK_SET);

    /* and read the returned number of bytes */
    char *buff = NULL;
    if ((buff = realloc(buff, size)) == NULL)
        error_and_exit("realloc failed in extract_file");
    fread(buff, size, 1, archive_file); /*error msgs on all f* */
    FILE *output_file = fopen(filename, "w");
    fwrite(buff, size, 1, output_file);
    fclose(output_file);
}

void
extract_files(char *archive_name, char **filelist, int num_entries)
{
    /* open archive file for read, "w" would be for write */
    FILE *archive_file = (strcmp(archive_name, "-")==0) ? stdin : fopen(archive_name, "r");
    if (archive_file == NULL)
        error_and_exit("fopen failed reading archive file - exiting");

    /* read in the table of contents */
    header_t header = read_header(archive_file);

    if (filelist[0] != NULL){
        for (int j=0; j<num_entries; j++){
            extract_file(filelist[j], archive_file, header);   
        }
    }
    else {
        for (int j=0; j<header.num_entries; j++){
            extract_file(header.toc[j].file_name, archive_file, header); 
        }        
    }

    fclose(archive_file);
}

int
main(int argc, char *argv[])
{
    int verbose_flag = 0;

    if (argc == 1) {
        fprintf(stderr, "Usage: %s (c|t|x) file.rar [file1 file2 file...]\n", argv[0]);
        exit(-1);
    }

    char *option = argv[1];
    char *archive_name = argv[2];
    char **filelist = &argv[3]; 

    if (strcmp(option, "-t") == 0)
        list_TOC(archive_name);
    else if (strcmp(option, "-c") == 0)
        create_archive(archive_name, filelist, argc-3);
    else if (strcmp(option, "-x") == 0)
        extract_files(archive_name, filelist, argc-3);
    else
        fprintf(stderr, "%s: Unrecognized option: '%s'\n", argv[0], option);

    return 0;
}
