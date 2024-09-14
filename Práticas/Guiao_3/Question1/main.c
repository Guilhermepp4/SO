#include <stdio.h>
#include <unistd.h>

int main(){
    execlp("ls", "ls", "-l", NULL);
    printf("HERE!!!");
    return 0;
}