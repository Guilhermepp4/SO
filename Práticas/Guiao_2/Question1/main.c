#include <stdio.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */

int main(){
    
    pid_t pid = getpid();
    pid_t ppid = getppid();

    printf("Mine: %d\n"
    "Father %d\n", pid, ppid);
    
    return 0;
}