#include <stdio.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */

int main(){

    int pid = fork();

    if(pid == 0){

        pid = getpid();
        printf("MineS: %d\n", pid);
        printf("FatherS: %d\n", getppid());

    } else {
        
        wait(NULL);
        printf("MineF: %d\n", getpid());
        printf("SonF: %d\n", pid);
        printf("FatherF: %d\n", getppid());
    }
    return 0;
}