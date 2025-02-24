#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "defs.h"

#define fifo_name "myfifo"

// Só podemos por o client a correr depois de ter o server aberto

int main (int argc, char * argv[]){

	if (argc < 2) {
		printf("Missing argument.\n");
		_exit(1);
	}
	int fd;
	int number = atoi(argv[1]);
	int resp;

	fd = open(fifo_name, O_WRONLY);
	if(fd == -1) return -1;

	if(write(fd, &number, sizeof(int)) == -1) return -1;

	close(fd);

	fd = open(fifo_name, O_RDONLY);
	if(fd == -1) return -1;
	if((read(fd, &resp, sizeof(int))) == -1) return -1;

	char buffer[100];
	sprintf(buffer, "The number of occurrences: %d\n", resp);

	if(write(1, buffer, strlen(buffer)) == -1) return -1;

	close(fd);
	unlink(fifo_name);

	return 0;
}