#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 100

int main(){
    int fd;                   
    ssize_t bytes_lidos;      
    char buffer[BUFFER_SIZE]; 

    // Abre o FIFO (pipe nomeado) "myfifo" em modo de escrita (O_WRONLY)
    if((fd = open("myfifo", O_WRONLY)) == -1) return -1;

    // Lê dados da entrada padrão (teclado, descritor 0) para o buffer
    if((bytes_lidos = read(0, buffer, BUFFER_SIZE)) == -1) return -1;

    // Escreve os dados lidos no FIFO
    // Utiliza "bytes_lidos" para garantir que todos os dados lidos sejam escritos
    if(write(fd, buffer, bytes_lidos) == -1) return -1;

    close(fd);

    return 0;
}