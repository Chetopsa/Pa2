#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

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
    char* buffer = (char*)malloc(PATH_MAX + 528*5);
    //TODO(step3): open directory
    DIR *dir = opendir(dir_path);
    struct dirent *entry;

    //TODO(step4): traverse directory and fork child process
    // Hints: Maintain an array to keep track of each read end pipe of child process

    //create pipe
    int fd[2];
    int ret = pipe(fd);
    if(ret == -1){
        printf("Error creating pipe\n");
        exit(-1);
    }
    while((entry = readdir(dir)) != NULL){
        // skip . and ..
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
            continue;
        }
        // create stat struct
        struct stat *stratbuf = malloc(sizeof(struct stat));
        if(lstat(entry->d_name, stratbuf)){
            perror("lstat");
            free(stratbuf);
            continue;
        }
        //get full path-name
        char full_path[PATH_MAX];
        sprintf(full_path, "%s/%s", dir_path, entry->d_name);
        
        //fork process
        pid = fork();

        if(pid == 0){
            if (S_ISDIR(stratbuf->st_mode)){
                free(stratbuf);
                execl("./nonleaf_process", "Pa2/nonleaf_process", full_path, fd[1], NULL);
            }else {
                free(stratbuf);
                execl("./leaf_process", "Pa2/leaf_process", full_path, fd[1], NULL);
            }
        }else{
            wait(NULL);
            close(fd[1]);
        }
    }
    
    //TODO(step5): read from pipe constructed for child process and write to pipe constructed for parent process
    ssize_t bytes = read(fd[0], buffer, sizeof(buffer));
    close(fd[0]);
    write(pipe_write_end, buffer, bytes);
    close(pipe_write_end);

    //free any heap allocated variables
    free(buffer);
}
