#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(){
    int fd[2];
    pid_t pid;
    pid_t pid2;

    if(pipe(fd) == -1) return -1;

    if((pid = fork()) == -1) return -1;

    //First Son Process
    if(pid == 0){

        close(fd[0]);

        dup2(fd[1], 1);
        close(fd[1]);

        execlp("ls", "ls", "/etc", NULL);
    } else {
        if((pid2 = fork()) == -1) return -1;
        
        //Second Son Process
        if(pid2 == 0){
            close(fd[1]);

            dup2(fd[0], 0);
            close(fd[0]);

            execlp("wc", "wc", "-l", NULL);
        }
    }

    //Father's Process
    close(fd[0]);
    close(fd[1]);

    wait(NULL);
    wait(NULL);
    
    printf("Terminei de ler as linhas\n\n");
    return 0;
}