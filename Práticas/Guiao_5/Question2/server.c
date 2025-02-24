#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "defs.h"
#include "vector.h"

//FIFO criado pelo servidor
//Cliente pode receber um sigpipe (concorrência!)	//printf("Escrevi o valor %d para o fifo\n\n",resp);
#define fifo_name "myfifo"

int main (int argc, char * argv[]){

	init_vector();
	print_vector();

	mkfifo(fifo_name, 0666);

	int number;	
	int fd;
	int resp;

	fd = open(fifo_name, O_RDONLY);
	if(fd == -1) return -1;

	if(read(fd, &number, sizeof(int)) == -1) return -1;
	resp = count_needle(number);
	close(fd);

	fd = open(fifo_name, O_WRONLY);
	if(fd == -1) return -1;
	if (write(fd, &resp, sizeof(int)) == -1) return -1;
	
	close(fd);
	return 0;
}