#include <sys/types.h>
#include <stdio.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */

#define BUF_SIZE 100

int main(){

    ssize_t read_bytes;

    int fd_original = open("original.txt", O_RDONLY);

    char buffer[BUF_SIZE];
    int fd_destination = open("final.txt", O_WRONLY | O_TRUNC | O_CREAT, 0600);

    if(fd_destination == -1){
        printf("Error creating the new file");
        return 2;
    }

    while((read_bytes = read(fd_original,buffer,BUF_SIZE)) > 0){
        if(write(fd_destination,buffer,read_bytes) == -1){
            printf("Error writing in the final file");
            return 1;
        }

    }
    close(fd_destination);
    close(fd_original);
    return 0;

}