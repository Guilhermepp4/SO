#include "matrix.h"

// A estrutura base para os exercícios 5 e 6 foi publicada juntamente com o guião!!!
int main(int argc, char *argv[]) {

    // generate random matrix
    int **matrix = createMatrix();

    // print matrix
    printMatrix(matrix);

    if(valueExists(matrix, 10)){
        linesWithValue(matrix, 10);
    }

    // free matrix
    for (int i = 0; i < ROWS; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}