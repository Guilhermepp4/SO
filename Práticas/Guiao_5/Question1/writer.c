#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 100

int main(){
    int fd;                
    char buffer[BUFFER_SIZE];
    ssize_t bytes_lidos;      

    // Abre o FIFO (pipe nomeado) "myfifo" em modo de leitura (O_RDONLY)
    if((fd = open("myfifo", O_RDONLY)) == -1) return -1;

    // Lê continuamente do FIFO enquanto houver dados a serem lidos
    while((bytes_lidos = read(fd, buffer, BUFFER_SIZE)) > 0){
        // Escreve os dados lidos no FIFO para a saída padrão (descritor 1)
        if(write(1, buffer, bytes_lidos) == -1) return -1;
    }

    close(fd);

    // Remove o FIFO do sistema de arquivos
    unlink("myfifo");

    return 0;
}