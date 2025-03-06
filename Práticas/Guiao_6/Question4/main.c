#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int fd[2];
    if(pipe(fd) == -1) return -1;

    if((pid = fork()) == -1) return -1;

    if(pid == 0){
        close(fd[1]);

        dup2(fd[0],0);
        close(fd[0]);
        execlp("wc", "wc", NULL);

    }

    if(close(fd[0]) == -1) return -1;

    char buffer[10];
    ssize_t bytes_lidos;

    while((bytes_lidos = read(1, buffer, sizeof(buffer))) > 0){
        write(fd[1], buffer, bytes_lidos);
    }

    close(fd[0]);
    wait(NULL);
    
    printf("\nTerminei\n");
    return 0;
}
