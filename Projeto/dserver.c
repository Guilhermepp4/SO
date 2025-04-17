#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include "program.h"

#define fifoName "myfifo"
char fifo_resposta[64];


MetaInfo documentos[MAX_DOCS];
int next_id = 1;
int num_documentos = 0;

void add(char* buffer) {
    int fifo;
    MetaInfo m;
    char resposta[560];

    // Gerar ID automaticamente (pode ser substituído por algo mais complexo)
    snprintf(m.id, MAX_ID, "doc%d", next_id++);
    printf("%s\n\n", buffer);
    // Parsing do buffer (esperado: "ADD|title|authors|year|path")
    char* token = strtok(buffer, "|"); // token = "ADD" (ignoramos)

    token = strtok(NULL, "|"); // title
    if (token != NULL) strncpy(m.title, token, MAX_TITLE);

    token = strtok(NULL, "|"); // authors
    if (token != NULL) strncpy(m.authors, token, MAX_AUTHORS);

    token = strtok(NULL, "|"); // year
    if (token != NULL){ 
        strncpy(m.year, token, MAX_YEAR); 
        m.year[MAX_YEAR - 1] = '\0'; 
    }

    token = strtok(NULL, "|"); // path
    if (token != NULL) strncpy(m.path, token, MAX_PATH);
    
    token = strtok(NULL, "|"); // fifo_resposta
    
    if((fifo = open(token, O_WRONLY)) == -1){
        perror("Erro ao abrir o fifo da reposta para escrever\n");
        return;
    }
    
    snprintf(resposta,sizeof(resposta), 
            "Documento indexado com sucesso:\n" 
            "ID: %s\n"
            "Título: %s\n"
            "Autores: %s\n"
            "Ano: %s\n"
            "Caminho: %s\n\n", m.id, m.title, m.authors, m.year, m.path);
    
    write(fifo, resposta, strlen(resposta));
    
    close(fifo);
    if(num_documentos < MAX_DOCS){
        documentos[num_documentos++] = m;
    }
}

void consult(char* buffer){
    char resposta[600];
    char* token = strtok(buffer, "|"); // token = "Consult" (ignoramos)
    token = strtok(NULL, "|"); // ID
    char* fifo_resposta = strtok(NULL, "|"); // token = "Consult" (ignoramos)
    int fifo;
    
    if((fifo = open(fifo_resposta, O_WRONLY)) == -1){
        perror("Erro ao abrir o fifo da reposta para escrever\n");
        return;
    }

    for(int i = 0; i < num_documentos; i++){
        if(strcmp(token, documentos[i].id) == 0){
            snprintf(resposta,sizeof(resposta), 
            "Aqui está a meta informação do documento com o ID %s\n" 
            "ID: %s\n"
            "Título: %s\n"
            "Autores: %s\n"
            "Ano: %s\n"
            "Caminho: %s\n\n", token, documentos[i].id, 
            documentos[i].title, documentos[i].authors
            , documentos[i].year, documentos[i].path);

            write(fifo, resposta, strlen(resposta));
    
            close(fifo);
            return;
        }
    }
    printf("Não existe meta informação relativa ao documento com o ID %s :(\n", token);
    printf("Tente com um ID diferente\n\n");
}

void delete(char *buffer) {
    char resposta[560];
    int fifo;
    char* token = strtok(buffer, "|"); // token = "Remove" (ignoramos)
    token = strtok(NULL, "|"); // ID
    char* fifo_resposta = strtok(NULL, "|"); // token = "Remove" (ignoramos)

    if((fifo = open(fifo_resposta, O_WRONLY)) == -1){
        perror("Erro ao abrir o fifo da reposta para escrever\n");
        return;
    }

    // Verificar e remover o documento com o ID
    for (int i = 0; i < num_documentos; i++) {
        if (strcmp(documentos[i].id, token) == 0) {
            // Encontramos o documento com o ID
            for (int j = i; j < num_documentos - 1; j++) {
                documentos[j] = documentos[j + 1];
            }
            num_documentos--;
            snprintf(resposta, sizeof(resposta), "Documento com ID %s removido com sucesso.\n\n", token);
            write(fifo, resposta, strlen(resposta));
    
            close(fifo);
            return;
        }
    }
    snprintf(resposta, sizeof(resposta), "Documento com ID %s não encontrado.\n\n", token);
    write(fifo, resposta, strlen(resposta));
    
    close(fifo);
}

void count(char* buffer){
    char resposta[560];
    char* token = strtok(buffer, "|"); // Ignora "Numero de Linhas"
    token = strtok(NULL, "|");  // ID do documento
    char* palavra = strtok(NULL, "|"); // Palavra a procurar
    char* fifo_resposta = strtok(NULL, "|");
    int fifo;

    if((fifo = open(fifo_resposta, O_WRONLY)) == -1){
        perror("Erro ao abrir o fifo da reposta para escrever\n");
        return;
    }
    for (int i = 0; i < num_documentos; i++) {
        if (strcmp(documentos[i].id, token) == 0) {
            // Primeiro, verifica se a palavra existe
            pid_t grep_pid = fork();
            int status;

            if (grep_pid == 0) {
                execlp("grep", "grep", "-q", palavra, documentos[i].path, NULL);
                _exit(1); // grep falhou
            }

            wait(NULL);

            if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                // Palavra existe → contar linhas
                int fd[2];
                if (pipe(fd) == -1) {
                    perror("pipe");
                    return;
                }

                pid_t wc_pid = fork();
                if (wc_pid == 0) {
                    close(fd[0]);
                    dup2(fd[1], STDOUT_FILENO);
                    close(fd[1]);

                    char comando[256];
                    snprintf(comando, sizeof(comando), "wc -l < %s", documentos[i].path);
                    execlp("sh", "sh", "-c", comando, NULL);

                    perror("wc falhou");
                    _exit(1);
                } else {
                    close(fd[1]);
                    char output[128];
                    ssize_t n = read(fd[0], output, sizeof(output));
                    if (n > 0) {
                        output[n-1] = '\0';
                        snprintf(resposta, sizeof(resposta), "Documento com ID %s tem %s linhas.\n", token, output);
                        write(fifo, resposta, strlen(resposta));
    
                        close(fifo);
                    }
                    close(fd[0]);
                    wait(NULL);
                }
            } else {
                snprintf(resposta, sizeof(resposta), "Palavra '%s' não encontrada no documento com ID %s.\n", palavra, token);
                write(fifo, resposta, strlen(resposta));
    
                close(fifo);
            }

            return;
        }
    }
    snprintf(resposta, sizeof(resposta), "Documento com ID %s não encontrado.\n", token);
    write(fifo, resposta, strlen(resposta));
    
    close(fifo);
            
}


void list(char* buffer) {
    int fifo;
    char resposta[560];
    char* token = strtok(buffer, "|"); // Ignora "ListDocs"
    token = strtok(NULL, "|");
    char* limite_str = strtok(NULL, "|");
    char* fifo_resposta = strtok(NULL, "|");

    int max_procs = (limite_str != NULL) ? atoi(limite_str) : 1;
    int active_procs = 0;

    if((fifo = open(fifo_resposta, O_WRONLY)) == -1){
        perror("Erro ao abrir o fifo para escrever\n");
        return;
    }
    snprintf(resposta, sizeof(resposta), "Lista de Documentos que contem a palavra %s\n\n", token);
    write(fifo, resposta, strlen(resposta));
    for (int i = 0; i < num_documentos; i++) {
        int fd[2];
        if (pipe(fd) == -1) {
            perror("Erro ao criar pipe");
            continue;
        }

        pid_t pid = fork();

        if (pid == -1) {
            perror("Erro no fork");
            continue;
        }

        if (pid == 0) {
            // Processo filho
            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);

            char comando[256];
            snprintf(comando, sizeof(comando), "grep -o '%s' '%s' | wc -l", token, documentos[i].path);
            execlp("sh", "sh", "-c", comando, NULL);
            _exit(1);
        } else {
            // Processo pai
            active_procs++;

            if (active_procs >= max_procs) {
                wait(NULL); // Espera 1 processo terminar
                active_procs--;
            }
            

            close(fd[1]);
            char buffer_saida[128];
            int n = read(fd[0], buffer_saida, sizeof(buffer_saida) - 1);
            if (n > 0) {
                buffer_saida[n] = '\0';
                int ocorrencias = atoi(buffer_saida);
                if (ocorrencias > 0) {
                    snprintf(resposta, sizeof(resposta), "%s\n", documentos[i].id);
                    write(fifo, resposta, strlen(resposta));
                }
            }
            close(fd[0]);
        }
    }

    // Esperar por todos os restantes processos
    while (active_procs > 0) {
        wait(NULL);
        active_procs--;
    }
    close(fifo);
}

void verifica_comandos(char* buffer) {

    // Verifica se é o comando '-a'
    if (strstr(buffer, "ADD") != NULL) {
        printf("Comando de adição detetado\n\n");
        add(buffer);
    }
    // Verifica se é o comando '-c' 
    else if (strstr(buffer, "Consulta") != NULL) {
        printf("Comando de consulta detetado.\n");
        consult(buffer);
    }
    // Verifica se é o comando '-d' 
    else if (strstr(buffer, "Remove") != NULL) {
        printf("Comando de remoção detetado.\n");
        delete(buffer);
    }
    // Verifica se é o comando '-l' 
    else if (strstr(buffer, "NumberLines") != NULL) {
        printf("Comando para contar linhas detetado.\n");
        count(buffer);
    }
    // Verifica se é o comando '-s' 
    else if (strstr(buffer, "ListDocs") != NULL) {
        printf("Comando para listar documentos detetado.\n");
        list(buffer);
    }
    // Verificar se o comando
    else {
        printf("Comando não reconhecido: %s\n", buffer);
    }
}

void fifo(){

    if (mkfifo(fifoName, 0666) == -1 && errno != EEXIST) {
        perror("Erro ao criar o FIFO"); 
        return; 
    }
    printf("Servidor a correr. À espera de pedidos...\n");
    while (1) {
        int fd;
        ssize_t bytes_lidos;
        char buffer[512];

        if((fd = open(fifoName, O_RDONLY)) == -1){
            perror("Erro ao abrir o fifo para ler\n");
            return;
        }
        
        if((bytes_lidos = read(fd, buffer, sizeof(buffer))) == -1){
            perror("Erro ao ler do FIFO");
            close(fd);
            return;
        }
        if (bytes_lidos > 0) {
            buffer[bytes_lidos] = '\0';  // Garantir terminação nula        
            // Remover possíveis caracteres de nova linha
            if (buffer[bytes_lidos - 1] == '\n') {
                buffer[bytes_lidos - 1] = '\0';
            }
        }

        verifica_comandos(buffer);
        close(fd);
    }
}

void cleanup(int sig) {
    unlink(fifoName);
    printf("\nServidor terminou. FIFO removido.\n");
    exit(0);
}

int main() {
    // Registar handler para CTRL+C (SIGINT)
    signal(SIGINT, cleanup);
    fifo();
    return 0;
}
