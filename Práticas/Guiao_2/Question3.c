#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main() {
    int status;
    int i = 1;
    pid_t pid;

    while(i < 11){
        if(fork() == 0){
            printf("Father: %d\n", getppid());
            printf("Son: %d\n", getpid());
            _exit(i);
        } else {
            pid = wait(&status);
            printf("The process with the number %d just finished with the pid %d\n\n", WEXITSTATUS(status), pid);
        }
        i++;
    }
    return 0;
}