#include <stdio.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */

int main(){

    int pid = fork();
    pid_t pd;
    int status;

    if(pid == 0){
        printf("Son's Process\n\n");
        printf("Mine: %d\n"
        "Father %d\n\n", getpid(), getppid());
        _exit(0);
    } else {
        pd = wait(&status);
        printf("Father's Process\n\n");
        printf("Mine: %d\n"
        "Father: %d\n"
        "Son: %d\n", getpid(), getppid(), pd);
    }

    return 0;
}