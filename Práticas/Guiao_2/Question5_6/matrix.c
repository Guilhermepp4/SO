#include "matrix.h"

int **createMatrix() {

    // seed random numbers
    srand(time(NULL));

    // Allocate and populate matrix with random numbers.
    printf("Generating numbers from 0 to %d...", MAX_RAND);
    int **matrix = (int **) malloc(sizeof(int*) * ROWS);
    for (int i = 0; i < ROWS; i++) {
        matrix[i] = (int*) malloc(sizeof(int) * COLUMNS);
        for (int j = 0; j < COLUMNS; j++) {
            matrix[i][j] = rand() % MAX_RAND;
        }
    }
    printf("Done.\n");

    return matrix;
}

void printMatrix(int **matrix) {

    for (int i = 0; i < ROWS; i++) {
        printf("%2d | ", i);
        for (int j = 0; j < COLUMNS; j++) {
            printf("%7d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// Função que verifica se um valor existe na matriz
int valueExists(int **matrix, int value) {
    int status;
    int run = 0; 

    // Cria processos filhos para procurar o valor em cada linha
    for (int i = 0; i < ROWS; i++) {
        if (fork() == 0) { 
            // Percorre a linha i para procurar o valor
            for (int j = 0; j < COLUMNS; j++) {
                if (value == matrix[i][j]) {
                    _exit(i);  // Se encontrar o valor, retorna o índice da linha
                }
            }
            _exit(255);  // Se não encontrar o valor, sai com código de erro -1
        }
    }

    // O processo pai espera pelos processos filhos
    for (int i = 0; i < ROWS; i++) {
        pid_t pid = wait(&status);  // Aguarda o término de um processo filho
        int final = WEXITSTATUS(status);  // Recupera o valor de saída do processo filho

        if (final != 255) {  // Se o processo filho encontrou o valor
            printf("The number %d is on the line %d\n"
                   "Pid: %d\n\n", value, final, pid);  // Exibe a linha e o PID do processo
            run = 1;  // Indica que o valor foi encontrado
        } else if (i == ROWS - 1 && run == 0) {  // Se todos os processos terminaram e o valor não foi encontrado
            printf("The number was not found :(\n");  
        }
    }

    return run;  
}

// Função de comparação para o qsort (ordena inteiros de forma crescente)
int compare(const void *a, const void *b){
    return *(int*)a - *(int*)b;
}

// Função que ordena uma linha da matriz
void sortLine(int *list, int length, int number) {
    qsort(list, length, sizeof(int), compare);
    printf("Order line %d\n\n", number);

    for (int i = 0; i < COLUMNS; i++){
        printf("%d ", list[i]);
    } 
    printf("\n\n");
}

// Função que busca o valor na matriz, e se encontrar, ordena a linha correspondente
void linesWithValue(int **matrix, int value) {

    pid_t pid;
    int list[COLUMNS];

    for(int i = 0;i < ROWS; i++){
        if((pid = fork()) == -1) _exit(-1);
    //Son's Process
        if(pid == 0){
            for(int j = 0; j < COLUMNS; j++){
                if(matrix[i][j] == value){
                    memcpy(list, matrix[i], COLUMNS * sizeof(int));
                    sortLine(list, COLUMNS, i);
                    _exit(i);
                }
            }
            _exit(-1);
        }
    }
    for(int i = 0; i < ROWS; i++){
        wait(NULL);
    }
}