#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
    
    pid_t pid;

    int file1 = open("/etc/passwd", O_RDONLY, 0666);
    int file2 = open("saida.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    int file3 = open("erros.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);

    int final = dup(1);

    dup2(file1,0);
    if(close(file1) == -1) return -1;

    dup2(file2,1);
    if(close(file2) == -1) return -1;

    dup2(file3,2);    
    if(close(file3) == -1) return -1;

    char buffer[1024];
    ssize_t bytes_lidos;

    while((bytes_lidos = read(0, buffer, 1024)) > 0){
        write(1, buffer, bytes_lidos);
        write(2, buffer, bytes_lidos);
    }

    dup2(final, 1);
    if(close(final) == -1) return -1;
    printf("Terminei\n");

    return 0;
}