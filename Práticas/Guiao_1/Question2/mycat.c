#include <sys/types.h>  
#include <stdio.h>      
#include <unistd.h>     
#include <fcntl.h>      

#define BUF_SIZE 100   

int main() {
    ssize_t read_bytes, finished_bytes; // Variáveis para armazenar o número de bytes lidos e escritos
    char buffer[BUF_SIZE];              // Buffer para armazenar os dados lidos

    // Loop que continua enquanto houver bytes para ler da entrada padrão (file descriptor 0)
    while ((read_bytes = read(0, buffer, BUF_SIZE)) > 0) {
        // Exibe a quantidade de bytes lidos da entrada padrão
        printf("Readed %ld bytes\n", read_bytes);

        // Escreve os bytes lidos no buffer para a saída padrão (file descriptor 1)
        finished_bytes = write(1, buffer, read_bytes);

        // Exibe a quantidade de bytes escritos na saída padrão
        printf("Escrevi %ld bytes\n", finished_bytes);

        // Verifica se o número de bytes escritos é diferente do número de bytes lidos
        if (finished_bytes != read_bytes) {
            // Se houver uma discrepância, exibe uma mensagem de erro
            printf("Error: Unable to write to the final file\n");
            return 1;  
        }
    }

    return 0;  
}
