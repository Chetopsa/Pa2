#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/hash.h"
#include "../include/utils.h"

char *output_file_folder = "output/inter_submission/";

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: Inter Submission --> ./leaf_process <file_path> 0\n");
        printf("Usage: Final Submission --> ./leaf_process <file_path> <pipe_write_end>\n");
        return -1;
    }
    //TODO(): get <file_path> <pipe_write_end> from argv[]
    char* file_path = argv[1];
    int pipe_write_end = atoi(argv[2]);
    //TODO(): create the hash of given file
    char file_hash[SHA256_BLOCK_SIZE * 2 + 1];
    hash_data_block(file_hash, file_path);
    //TODO(): construct string write to pipe. The format is "<file_path>|<hash_value>"
    char str_pipe[PATH_MAX + SHA256_BLOCK_SIZE * 2 + 2];
    sprintf(str_pipe,"%s|%s|" , file_path, file_hash);

    if(pipe_write_end == 0){
        //TODO(inter submission)
        //TODO(overview): create a file in output_file_folder("output/inter_submission/root*") and write the constructed string to the file
        //TODO(step1): extract the file_name from file_path using extract_filename() in utils.c
        char* file_name = extract_filename(file_path);
        //TODO(step2): extract the root directory(e.g. root1 or root2 or root3) from file_path using extract_root_directory() in utils.c
        char* root_directory = extract_root_directory(file_path);
        //TODO(step3): get the location of the new file (e.g. "output/inter_submission/root1" or "output/inter_submission/root2" or "output/inter_submission/root3")
        char new_file_location[PATH_MAX];
        sprintf(new_file_location, "%s%s%s", output_file_folder, root_directory, file_name);
        //TODO(step4): create and write to file, and then close file
        FILE* fptr = fopen(new_file_location, "w");
        if (fptr == NULL) {
            perror("Error opening file");
            return -1;
        }
        fprintf(fptr, "%s", str_pipe);
        fclose(fptr);
        //TODO(step5): free any arrays that are allocated using malloc!! Free the string returned from extract_root_directory()!! It is allocated using malloc in extract_root_directory()
        free(root_directory);

    }else{
        //TODO(final submission): write the string to pipe
        //write to pipe
        write(pipe_write_end, str_pipe, strlen(str_pipe));
        close(pipe_write_end); //close pipe
        
    }
    exit(0);
}
