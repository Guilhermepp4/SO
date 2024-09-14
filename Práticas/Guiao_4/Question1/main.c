#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>


int main(){
    int fd[2];
    pid_t pid;

    if(pipe(fd) == -1) return -1;

    if((pid = fork()) == -1) return -1;

    if(pid == 0){
        if(close(fd[1]) == -1) return -1;
        
        int x;
        if(read(fd[0], &x, sizeof(int)) == -1) return -1;
        printf("Son process, %d, readed from, %d, this number %d\n", getpid(), getppid(), x);
        
        if(close(fd[0]) == -1) return -1;
    } else {
        sleep(2);
        if(close(fd[0]) == -1) return -1;
        
        srand(time(NULL));
        int y = rand()%100;
        
        if(write(fd[1], &y, sizeof(int)) == -1) return -1;
        printf("Father process, %d, written this number %d\n\n", getpid(), y);

        if(close(fd[1]) == -1) return -1;
        wait(NULL);
    }
    return 0;
}