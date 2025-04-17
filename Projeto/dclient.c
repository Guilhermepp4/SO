#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "program.h"

#define fifoName "myfifo"
char fifo_resposta[64];

int fifo(char *mensagem){
    int fd;
    
    // Abrir o pipe para escrever
    if((fd = open(fifoName, O_WRONLY)) == -1){
        perror("Erro ao abrir o fifo para escrever\n");
        return 1;
    }
    write(fd, mensagem, strlen(mensagem));
    memset(mensagem, 0, 512);
    
    close(fd);
    return 1;
}

void construir_mensagem_add(char* mensagem, char** argv) {
    sprintf(mensagem, "ADD|%s|%s|%s|%s|%s\n",
            argv[2], argv[3], argv[4], argv[5], fifo_resposta);
}

void construir_mensagem_consulta(char* mensagem, char** argv) {
    sprintf(mensagem, "Consulta|%s|%s\n", argv[2], fifo_resposta);
}

void construir_mensagem_remove(char* mensagem, char** argv) {
    sprintf(mensagem, "Remove|%s|%s\n", argv[2], fifo_resposta);
}

void construir_mensagem_numero_linhas(char* mensagem, char** argv) {
    sprintf(mensagem, "NumberLines|%s|%s|%s\n", argv[2], argv[3], fifo_resposta);
}

void construir_mensagem_listar(char* mensagem, char** argv) {
    sprintf(mensagem, "ListDocs|%s|%s|%s\n", argv[2], argv[3], fifo_resposta);
}

void construir_mensagem_fechar(char* mensagem) {
    sprintf(mensagem, "Fechar");
    if (unlink(fifo_resposta) == -1) {
        perror("Erro ao remover o FIFO");
    } else {
        printf("FIFO removido com sucesso.\n");
    }
}

int ler_comandos(int argc, char** argv){
    char mensagem[512];
    
    if(argc < 2){
        printf("Erro na escrita de comandos!!\n");
        return -1;
    }

    sprintf(fifo_resposta, "/tmp/cliente_%d_fifo", getpid());
    if (mkfifo(fifo_resposta, 0666) == -1 && errno != EEXIST) {
        perror("Erro ao criar o FIFO"); 
        return -1; 
    }
    
    // Se for o comando "-a"
    if (strcmp(argv[1], "-a") == 0 && argc == 6) {
        // mensagem = "ADD|title|authors|year|path"
        construir_mensagem_add(mensagem, argv);
    }
    // Se for o comando "-c"
    else if (strcmp(argv[1], "-c") == 0 && argc == 3) {
        // mensagem = "Consulta|Documento"
        construir_mensagem_consulta(mensagem, argv);
    }
    // Se for o comando "-d"
    else if (strcmp(argv[1], "-d") == 0 && argc == 3){
        // mensagem = "Remove|Documento"
        construir_mensagem_remove(mensagem, argv);
    }
    // Se for o comando "-l"
    else if (strcmp(argv[1], "-l") == 0 && argc == 4){
        // mensagem = "NumberLines|Documento|Word"
        construir_mensagem_numero_linhas(mensagem, argv);
    }
    // Se for o comando "-s"
    else if (strcmp(argv[1], "-s") == 0 && argc == 4){
        // mensagem = "ListDocs|Word|Limite|Fifo_name"
        construir_mensagem_listar(mensagem, argv);
    }
    // Se for o comando "-f"
    else if (strcmp(argv[1], "-f") == 0 && argc == 2){
        // mensagem = "Fechar"
        construir_mensagem_fechar(mensagem);
    }
    else{
        printf("Comando inválido!!\n");
        return -1;
    }
    fifo(mensagem);
    return 1;
}

int main(int argc, char** argv){
    int fd;
    ssize_t bytes_lidos;
    char buffer[512];

    ler_comandos(argc, argv);

    fd = open(fifo_resposta, O_RDONLY);
    while((bytes_lidos = read(fd, buffer, 512)) > 0){
        // Escreve os dados lidos no FIFO para a saída padrão (descritor 1)
        if(write(1, buffer, bytes_lidos) == -1) return -1;
    }
    close(fd);
    return 0;
}