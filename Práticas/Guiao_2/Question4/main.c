#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(){
    int status;

    for(int i = 0; i <= 10; i++){
        if(fork() == 0){
            if(i == 10){
                printf("Son Process %d\n\n", getpid());
                _exit(i);
            } else {
                printf("Son Process %d\n", getpid());
                _exit(i);
            }
        }
    }

    for (int i = 0; i <= 10; i++){
        int pid = wait(&status);
        if(i == 10){
            printf("The process with the number %d "
            "finished correctly, pid: %d\n\n", WEXITSTATUS(status), pid);
        } else {
            printf("The process with the number %d "
            "finished correctly, pid: %d\n", WEXITSTATUS(status), pid); 
        }
    }
    
    printf("Father: %d\n\n", getpid());

    return 0;
}