#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

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
    if((pid = fork()) == -1) return -1;

    //Son's Process
    if (pid == 0) {

        char buffer_filho[1024];
        char buffer_filho_send[512];
        ssize_t bytes_lidos_filho;
        
        int len = sprintf(buffer_filho_send, "\n\n                             Processo FILHO %d :\n\n", getpid());

        write(1, buffer_filho_send, len);
        write(2, buffer_filho_send, len);

        while((bytes_lidos_filho = read(0, buffer_filho, 1024)) > 0){
            write(1, buffer_filho, bytes_lidos_filho);
            write(2, buffer_filho, bytes_lidos_filho);
        }

        _exit(0);
    }


    wait(NULL);

    dup2(final, 1);
    if(close(final) == -1) return -1;
    printf("Terminei finalmente\n");

}