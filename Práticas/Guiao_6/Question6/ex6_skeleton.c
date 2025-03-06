#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>


#define MAX_COMMANDS 10

// parse do argumento e execvp
// devolve o resultado do exec
int exec_command(char* arg){

	//Estamos a assumir numero maximo de argumentos
	char *exec_args[10];

	char *string;	
	int exec_ret = 0;
	int i=0;

	char* command = strdup(arg);

	string=strtok(command," ");
	
	while(string!=NULL){
		exec_args[i]=string;
		string=strtok(NULL," ");
		i++;
	}

	exec_args[i]=NULL;
	
	exec_ret=execvp(exec_args[0],exec_args);
	
	return exec_ret;
}


int main(int argc, char** argv){
	
	int number_of_commands=4;
	
	char * commands[] = {
		"grep -v ^# /etc/passwd",
		"cut -f7 -d:",
		"uniq",
		"wc -l"
	};

	pid_t pid;
	int fd[number_of_commands-1][2];

	for(int i = 0; i <= number_of_commands - 1; i++){
		if(pipe(fd[i]) == -1) return -1;
		if((pid = fork()) == -1) return -1;
		
		if(pid == 0){
			if(i == 0){
				close(fd[i][0]);

				dup2(fd[i][1],1);
				close(fd[i][1]);

				exec_command(commands[i]);

			} else if (i == number_of_commands-1){
				dup2(fd[i-1][0],0);
				close(fd[i-1][0]);

				exec_command(commands[i]);

			} else {
				dup2(fd[i-1][0],0);
				close(fd[i-1][0]);
				
				dup2(fd[i][1],1);
				close(fd[i][1]);

				exec_command(commands[i]);
			}
		} else {
			if (i == number_of_commands - 1){
				close(fd[i - 1][0]);
                close(fd[i][1]); 
			}
			if (i > 0) {
                close(fd[i - 1][0]);
            }
            if (i < number_of_commands - 1) {
                close(fd[i][1]);
            }
		}
	}

	for(int i = 0; i < number_of_commands; i++){
		wait(NULL);
	}
	printf("\nTerminei!\n");
	return 0;
}