#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pessoa.h"

#define FILENAME "lista_pessoas.txt"

// Este ficheiro servirá apenas para definir as funções auxiliares que vamos utilizar na main 

int add_person(char *name, int age){

    int fd = open(FILENAME, O_CREAT|O_WRONLY|O_APPEND, 0600);
    if(fd == -1) return 1;

    struct Person p;
    strcpy(p.name, name);
    p.age = age;

    ssize_t writing_bytes = write(fd, &p, sizeof(Person));

    if(writing_bytes < 0){
        close(fd);
        return 1;
    }

    printf("name: %s\n"
    "age: %d\n", p.name, p.age);

    close(fd);
    
    return 0;
}

int list_people(int N) {
    int fd = open(FILENAME,O_RDONLY);
    if (fd == -1) return 1;
    struct Person p;

    for(int i = 0; i < N; i++){
        ssize_t final_bytes = read(fd, &p, sizeof(Person));
        if(final_bytes != sizeof(Person)) return 1;

        printf("Name: %s\n"
                "Age: %d\n", p.name, p.age);
    }
    close(fd);
    return 0;
}

int update_age(char *name, int age){

    int fd = open(FILENAME, O_RDWR);
    if(fd == -1) return 1;
    
    bool run = true;
    struct Person p;

    while (run){
        ssize_t final_bytes = read(fd, &p, sizeof(Person));
        if(final_bytes != sizeof(Person)){
            printf("No more persons to read\n");
            close(fd);
            return 1;
        }
        if(strcmp(p.name, name) == 0){
            p.age = age;
            
            if(lseek(fd,-sizeof(Person), SEEK_CUR) == -1){
                close(fd);
                return 1;
            }
            if(write(fd, &p, sizeof(Person)) == -1){
                close(fd);
                return 1;
            }
            run = false;
        }
    }
    
    close(fd);
    return 0;
}


int add_person_give_position(char *name, int age){

    int fd = open(FILENAME, O_RDWR);
    int i = 1;
    if(fd == -1) return 1;

    struct Person put;

    strcpy(put.name,name);
    put.age = age;

    Person p;
    
    ssize_t read_bytes;

    while((read_bytes = read(fd, &p, sizeof(Person))) > 0){
        if(read_bytes != sizeof(Person)) break;
        i++;
    }
    
    if(write(fd, &put, sizeof(Person)) == -1) {
        close(fd); 
        return 1;
    }
    
    printf("I have added a person:\n"
    "Name: %s\n"
    "Age: %d\n", put.name, put.age);

    close(fd);
    return i;
}

int update_age_by_num(int position, int age){

    int fd = open(FILENAME, O_RDWR);

    if(fd == -1) return 1;

    bool run = true;
    int i = 0;
    struct Person p;
    while (run){

        ssize_t read_bytes = read(fd,&p, sizeof(Person));
        if(read_bytes != sizeof(Person)) return 1;

        if (i == position){
            p.age = age;
            if(lseek(fd,-sizeof(Person), SEEK_CUR) == -1){ 
                close(fd);    
                return 1;
            }
            if(write(fd, &p, sizeof(Person)) == -1){
                close(fd);
                return 1;
            }
            run = false;
        }
        
        i++;
    }

    close(fd);
    return 0;
}