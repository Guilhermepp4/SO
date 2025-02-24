#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

#define number 100000

int main(){

    pid_t pid;
    int fd[2];

    if(pipe(fd) == -1) _exit(EXIT_FAILURE);

    if((pid = fork()) == -1) _exit(EXIT_FAILURE);

    if(pid == 0){
        
        if(close(fd[0]) == -1) _exit(EXIT_FAILURE);

        srand(time(NULL));
        int y = rand() % 100;
        
        for(int i = 0; i < number; i++){
            write(fd[1], &y, sizeof(int));
            printf("The process, %d, write the number %d, to the process %d\n\n", getpid(), y, getppid());
        }
        if(close(fd[1]) == -1) _exit(EXIT_FAILURE);
        
        _exit(0);
    }
    if(close(fd[1]) == -1) _exit(EXIT_FAILURE);
    
    sleep(2);
    int x;
    for(int i = 0; i < number; i++){
        if(read(fd[0], &x, sizeof(int)) == -1) _exit(EXIT_FAILURE);
        printf("The process, %d, read the number %d\n\n", getpid(), x);
    }
    
    if(close(fd[0]) == -1) _exit(EXIT_FAILURE);
    wait(NULL);

    _exit(0);
}
