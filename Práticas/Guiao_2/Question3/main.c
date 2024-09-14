#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main() {
    pid_t pid;
    int status;

    for (int i = 1; i < 11; i++){
        int pd = fork();
        
        if(pd == 0){
            printf("Mine: %d\n"
            "Father: %d\n", getpid(), getppid());
            _exit(i);
        } else{
            pid = wait(&status);
            printf("The process with the number %d just finished "
            "with the pid %d\n\n", WEXITSTATUS(status), pid);
        }
    }
    return 0;
}