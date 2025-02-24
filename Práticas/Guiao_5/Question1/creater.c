#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define fifoName "myfifo"

int main(){

    // Cria um FIFO com permissões de leitura e escrita (0666 -> rw-rw-rw-)
    // mkfifo() cria um arquivo especial que funciona como um pipe nomeado
    if (mkfifo(fifoName, 0666) == -1) {
        perror("Erro ao criar o FIFO"); 
        return 1; 
    }

    printf("FIFO: \"%s\" created!\n\n", fifoName);

    return 0;
}
