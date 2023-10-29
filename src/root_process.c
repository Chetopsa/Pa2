#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "../include/utils.h"
#include <sys/types.h>
#include <sys/wait.h>

#define WRITE (O_WRONLY | O_CREAT | O_TRUNC)
#define PERM (S_IRUSR | S_IWUSR)
char *output_file_folder = "output/final_submission/";

void redirection(char **dup_list, int size, char* root_dir){
    
    // TODO(overview): redirect standard output to an output file in output_file_folder("output/final_submission/")
    // TODO(step1): determine the filename based on root_dir. e.g. if root_dir is "./root_directories/root1", the output file's name should be "root1.txt"
    char output_file_name[PATH_MAX];
    char output_file_path[PATH_MAX];
    sprintf(output_file_name, "%s.txt", extract_root_directory(root_dir));


    //TODO(step2): redirect standard output to output file (output/final_submission/root*.txt)
    sprintf(output_file_path, "%s%s", output_file_folder, output_file_name);
   
    fflush(stdout); 
    int original_stdout = dup(STDOUT_FILENO);
    int fd = open(output_file_path, WRITE, PERM);
    if(fd == -1){
        perror("failed to open file\n");
        exit(-1);
    }
    if (dup2(fd, 1) == -1){
                    perror("Failed to redirect output\n");
                    exit(-1);
    }

    //TODO(step3): read the content each symbolic link in dup_list, write the path as well as the content of symbolic link to output file(as shown in expected)
    for(int i = 0; i < size; i++){
        char hard_path[PATH_MAX];
        memset(hard_path, 0, sizeof(hard_path)); 
        
        readlink(dup_list[i], hard_path, sizeof(hard_path)-1);
        printf("[<path of symbolic link> --> <path of retained file>] : [%s --> %s]\n", dup_list[i],  hard_path);
        
    }
    //resotore
    fflush(stdout);
    dup2(original_stdout, STDOUT_FILENO);
    close(original_stdout);
    close(fd);
}

void create_symlinks(char **dup_list, char **retain_list, int size) {
    //TODO(): create symbolic link at the location of deleted duplicate file
    //TODO(): dup_list[i] will be the symbolic link for retain_list[i]
    for(int i = 0; i < size; i++){
        symlink(retain_list[i], dup_list[i]);
    }
   
}

void delete_duplicate_files(char **dup_list, int size) {
    //TODO(): delete duplicate files, each element in dup_list is the path of the duplicate file
    for(int i = 0; i < size; i++){
        remove(dup_list[i]);
    }

}
// ./root_directories <directory>
int main(int argc, char* argv[]) {
    if (argc != 2) {
        // dir is the root_directories directory to start with
        // e.g. ./root_directories/root1
        printf("Usage: ./root <dir> \n");
        return 1;
    }

    //TODO(overview): fork the first non_leaf process associated with root directory("./root_directories/root*")

    char* root_directory = argv[1];
    char all_filepath_hashvalue[4098]; //buffer for gathering all data transferred from child process
    memset(all_filepath_hashvalue, 0, sizeof(all_filepath_hashvalue));// clean the buffer

    //TODO(step1): construct pipe
    int fd[2];
    int ret = pipe(fd);
    if(ret == -1){
        printf("Pipe open failure");
        exit(-1);
    }

    //TODO(step2): fork() child process & read data from pipe to all_filepath_hashvalue
    char pipe_write[8];
    sprintf(pipe_write, "%d", fd[1]);
    pid_t pid = fork();
    
    if(pid == 0){
        execl("./nonleaf_process", "PA2/nonleaf_process", root_directory, pipe_write, NULL);
    }

    else{
        wait(NULL);
        //TODO(step3): malloc dup_list and retain list & use parse_hash() in utils.c to parse all_filepath_hashvalue
        // dup_list: list of paths of duplicate files. We need to delete the files and create symbolic links at the location
        // retain_list: list of paths of unique files. We will create symbolic links for those files
        
        read(fd[0], all_filepath_hashvalue, 4098);
        char** dup_list = (char**)malloc(4098*60);
        char** retain_list = (char**)malloc(4098*60);

        int size = parse_hash(all_filepath_hashvalue, dup_list, retain_list);
        //TODO(step4): implement the functions
       
        delete_duplicate_files(dup_list,size);
        create_symlinks(dup_list, retain_list, size);
        redirection(dup_list, size, argv[1]);
        

        //TODO(step5): free any arrays that are allocated using malloc!!
        free(dup_list);
        free(retain_list);
        }
}
