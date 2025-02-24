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

void lookupNumber(int** matrix, int value, int* vector){

    pid_t pid;
    int fd[2];

    if(pipe(fd) == -1) _exit(-1);

    // Vamos criar um número de processos filho equivalentes ao número de linhas, cada processo filho
    //vai ser responsavel por procurar o valor pretendida numa linha em concreto
    for(int i = 0; i < ROWS; i++){

        if((pid = fork()) == -1) _exit(-1);

        if(pid == 0){

            int count = 0;
            Minfo mI;

            for(int j = 0; j < COLUMNS; j++){

                if(value == matrix[i][j]){
                    count ++;
                }
            }

            mI.line_nr = i;
            mI.ocur_nr = count;

            if(close(fd[0]) == -1) _exit(-1);
            /*Para conseguirmos transmitir mais dados do que apenas um valor vamos usar a struct Minfo tal 
            com é sugerido no enunciado, assim podemos enviar tanto a linha das ocurrencias como o 
            número de ocorrências assim não é um problema os processos filho percorrerem o programa 
            de forma concorrente pois não existirá mistura de informações
            */
            if(write(fd[1], &mI, sizeof(Minfo)) == -1) _exit(-1);
            if(close(fd[1]) == -1) _exit(-1);

            _exit(1);
        }    
    }  

    for(int i = 0; i < ROWS; i++){
        if(wait(NULL) == -1) _exit(-1);
    }

    if(close(fd[1]) == -1) _exit(-1);

    Minfo mI;

    // Tal como referi podemos utilizar a estratégia de ler até chegar ao EOF para valores indefinidos
    while(read(fd[0], &mI, sizeof(Minfo)) > 0){
        vector[mI.line_nr] = mI.ocur_nr;
    }
    
    if(close(fd[0]) == -1) _exit(-1);
}