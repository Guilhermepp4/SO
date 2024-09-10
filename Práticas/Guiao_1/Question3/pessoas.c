#include <sys/types.h>  
#include <fcntl.h>      
#include <unistd.h>     
#include <stdio.h>      
#include <string.h>     
#include <stdlib.h>     

#include "pessoa.h"     

// Este código apenas tem o propósito de interpretar os argumentos de entrada
int main(int argc, char *argv[]) {
    // Verifica se o número de argumentos passados é menor que 3
    if (argc < 3) {
        // Exibe instruções de uso caso o número de argumentos seja insuficiente
        printf("Usage:\n");
        printf("Add new person to the list: ./pessoas -i [name] [age]\n");
        printf("List N people: ./pessoas -l [N]\n");
        printf("Change person age: ./pessoas -u [name] [age]\n");
        printf("Change person age (v2): ./pessoas -o [position] [age]\n");
        return 1;  // Retorna 1 para indicar que ocorreu um erro devido ao uso incorreto
    }

    // Comando para adicionar uma nova pessoa à lista
    if (strcmp(argv[1], "-i") == 0) {
        // Converte a idade de string para inteiro usando atoi
        int final = add_person(argv[2], atoi(argv[3]));
        printf("Added %d person\n", final);  // Imprime o resultado da função add_person
    }

    if (strcmp(argv[1], "-i2") == 0) {
        int final = add_person_give_position(argv[2], atoi(argv[3]));
        printf("Updated %d person\n", final);  // Imprime a posição da pessoa adicionada
    }

    // Comando para listar N pessoas da lista
    if (strcmp(argv[1], "-l") == 0) {
        int final = list_people(atoi(argv[2]));  // Converte o argumento N para inteiro
        printf("Success!!! %d\n", final);  // Imprime o resultado da função list_people
    }

    if (strcmp(argv[1], "-u") == 0) {
        int final = update_age(argv[2], atoi(argv[3]));  // Converte a idade para inteiro
        printf("Updated the age of %d person\n", final);  // Imprime o resultado da função update_age
    }

    if (strcmp(argv[1], "-o") == 0) {
        int final = update_age_by_num(atoi(argv[2]), atoi(argv[3]));  // Converte a posição e a idade para inteiros
        printf("Updated the age of %d person\n", final);  // Imprime o resultado da função update_age_by_num
    }

    return 0;  
}
