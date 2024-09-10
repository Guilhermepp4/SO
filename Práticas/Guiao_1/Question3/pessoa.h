#include <unistd.h>  
#include <stdio.h>   
#include <fcntl.h>   
#include <string.h>  

// Definição de uma estrutura chamada 'Person' para armazenar dados de uma pessoa
typedef struct Person {
    char name[200];  // Array de caracteres para armazenar o nome da pessoa
    int age;         // Inteiro para armazenar a idade da pessoa
} Person;


// Função para adicionar uma pessoa com nome e idade ao armazenamento
int add_person(char *name, int age);

// Função para listar as N primeiras pessoas armazenadas
int list_people(int N);

// Função para atualizar a idade de uma pessoa com base no nome
int update_age(char *name, int age);

// Função para atualizar a idade de uma pessoa com base na posição na lista
int update_age_by_num(int position, int age);

// Função para adicionar uma pessoa e retornar a posição onde foi adicionada
int add_person_give_position(char *name, int age);

// Nota: As funções foram apenas declaradas, não implementadas. As implementações devem definir o comportamento descrito acima.
