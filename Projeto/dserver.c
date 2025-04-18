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
#define DATA_FILE "meta_info.txt"
char fifo_resposta[64];

int next_id = 1;
MetaInfo documentos[MAX_DOCS];
int num_documentos = 0;
int active_procs = 0;

void guardar_meta_info(MetaInfo* documentos, int total) {
    FILE *fp = fopen(DATA_FILE, "w");
    if (!fp) {
        perror("Erro ao guardar meta-informação");
        return;
    }
    for (int i = 0; i < total; i++) {
        fprintf(fp, "%s|%s|%s|%s|%s\n",
            documentos[i].id,
            documentos[i].title,
            documentos[i].authors,
            documentos[i].year,
            documentos[i].path);
    }
    fclose(fp);
}

int id_existe(const char* id) {
    for (int i = 0; i < num_documentos; i++) {
        if (strcmp(documentos[i].id, id) == 0) {
            return 1; // já existe
        }
    }
    return 0;
}

void gerar_id_unico(char* id_buffer) {
    do {
        snprintf(id_buffer, MAX_ID, "doc%d", next_id++);
    } while (id_existe(id_buffer));
}

//Comando "-a"
void add(char* buffer) {
    int fifo;
    MetaInfo m;
    char resposta[560];

    // Gerar ID automaticamente (pode ser substituído por algo mais complexo)
    gerar_id_unico(m.id);
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
        guardar_meta_info(documentos, num_documentos);
    }
}

//Comando "-c"
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

//Comando "-d"
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
            guardar_meta_info(documentos, num_documentos);
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

//Comando "-l"
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
                perror("grep falhou");
                _exit(1); // grep falhou
            }

            wait(&status);

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
                    int n = read(fd[0], output, sizeof(output));
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
                wait(NULL);
            }
            return;
        }
    }
    snprintf(resposta, sizeof(resposta), "Documento com ID %s não encontrado.\n", token);
    write(fifo, resposta, strlen(resposta));
    
    close(fifo); 
}

//Comando "-s"
void list(char* buffer) {
    int fifo;
    char resposta[560];
    char* token = strtok(buffer, "|"); // Ignora "ListDocs"
    token = strtok(NULL, "|");
    char* limite_str = strtok(NULL, "|");
    char* fifo_resposta = strtok(NULL, "|");

    int max_procs = atoi(limite_str);

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
            return;
        }

        if (pid == 0) {
            sleep(2);
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

int saida(){
    if (unlink(fifoName) == -1) {
        perror("Erro ao remover o FIFO");
    } else {
        printf("FIFO removido com sucesso.\n");
    }

    printf("Servidor a fechar");
    fflush(stdout);
    sleep(1);
    printf(".");
    fflush(stdout);
    sleep(1);
    printf(".");
    fflush(stdout);
    sleep(1);
    printf(".\n\n");
    sleep(1);

    printf("Servidor fechado com sucesso.\n");
    exit(0);
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
    // Verificar se é o comando '-f'
    else if (strstr(buffer, "Fechar") != NULL){
        printf("Comando para fechar servidor detetado.\n");
        saida();
    }
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

void carregar_meta_info() {
    FILE *fp = fopen(DATA_FILE, "r");
    if (!fp) return; // Se ainda não existir, continua sem erro

    char linha[512];
    while (fgets(linha, sizeof(linha), fp)) {
        MetaInfo d;
        sscanf(linha, "%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]",
            d.id, d.title, d.authors, d.year, d.path);
        documentos[num_documentos++] = d;
    }

    fclose(fp);
}

int main() {
    carregar_meta_info();
    fifo();
    return 0;
}