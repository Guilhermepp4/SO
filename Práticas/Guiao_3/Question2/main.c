#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


int main(){
    pid_t pid;

    if((pid = fork()) == -1) _exit(-1);

    if(pid == 0){
        execlp("ls", "ls", "-l", NULL);
    } else {
        wait(NULL);
        printf("\n\n");
        printf("I arrived here \n\n");
    }
    return 0;
}