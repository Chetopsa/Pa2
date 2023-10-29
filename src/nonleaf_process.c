#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

int BUFSIZE = 4098;
int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: ./nonleaf_process <directory_path> <pipe_write_end> \n");
        return 1;
    }
    pid_t pid;
    
    //TODO(overview): fork the child processes(non-leaf process or leaf process) each associated with items under <directory_path>
    //TODO(step1): get <file_path> <pipe_write_end> from argv[]
    char* dir_path = argv[1];
    int pipe_write_end = atoi(argv[2]);
    
    //TODO(step2): malloc buffer for gathering all data transferred from child process as in root_process.c
    char* buffer = (char*)malloc(BUFSIZE);
    memset(buffer, 0, BUFSIZE);
    //TODO(step3): open directory
    DIR *dir = opendir(dir_path);
    struct dirent *entry;

    //TODO(step4): traverse directory and fork child process
    // Hints: Maintain an array to keep track of each read end pipe of child process
    int* read_pipes = (int*)malloc(sizeof(int)*20);
    int pipe_idx = 0;
    //create pipe array
    int fd[2];
    while((entry = readdir(dir)) != NULL){
        // skip . and ..
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
            continue;
        }
        //create pipe 
        int ret = pipe(fd);
        if(ret == -1){
            printf("Error creating pipe\n");
            exit(-1);
        }
        //store read end in array
        
        //get full path-name
        char full_path[PATH_MAX];
        sprintf(full_path, "%s/%s", dir_path, entry->d_name);
        // create stat struct

        struct stat *stratbuf = malloc(sizeof(struct stat));
        if(lstat(full_path, stratbuf)){
            perror("lstat: nonleaf_p");
            free(stratbuf);
            continue;
        }

        char pipe_write[8];
        sprintf(pipe_write, "%d", fd[1]);
        //fork process
        pid = fork();
        if(pid == 0){
            close(fd[0]);
            if (S_ISDIR(stratbuf->st_mode)){
                free(stratbuf);
                execl("./nonleaf_process", "./nonleaf_process", full_path, pipe_write, NULL);
            }else {
                free(stratbuf);
                execl("./leaf_process", "./leaf_process", full_path, pipe_write, NULL);
            }
        }else{
            wait(NULL);
            close(fd[1]);
            read_pipes[pipe_idx++] = fd[0];
        }
       
    }
    closedir(dir);
    
    //TODO(step5): read from pipe constructed for child process and write to pipe constructed for parent process
    
    ssize_t bytes;
    
    for(int i = 0; i < pipe_idx; i ++){
        char pipe_data[4096];
        memset(pipe_data, 0, sizeof(pipe_data));
        //read from pipe
        bytes = read(read_pipes[i], &pipe_data, sizeof(pipe_data));
        //concatenate
        strncat(buffer, pipe_data, bytes);
        close(read_pipes[i]);
    }
    write(pipe_write_end, buffer, strlen(buffer));
    printf("Cummulative data length: %ld\n", strlen(buffer));
    close(pipe_write_end);
    
   
    //free any heap allocated variables
    free(buffer);
    free(read_pipes);

    printf("non_leaf success!\n ");
}