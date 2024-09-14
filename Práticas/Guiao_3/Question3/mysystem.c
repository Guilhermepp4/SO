#include "mysystem.h"


// recebe um comando por argumento
// returna -1 se o fork falhar
// caso contrario retorna o valor do comando executado
int mysystem (const char* command) {

	int res = -1;
	pid_t pid;
	int status;

	// Estamos a assumir numero maximo de argumentos
	// isto teria de ser melhorado com realloc por exemplo
	char *exec_args[20];
	char *string, *cmd, *tofree;
	int i=0;
	tofree = cmd = strdup(command);
	while((string = strsep(&cmd," "))!=NULL){
	   exec_args[i]=string;
	   i++;
	}
	exec_args[i]=NULL;

	if((pid = fork()) == -1) _exit(-1);

	if(pid == 0){
		execvp(exec_args[0], exec_args);
		_exit(EXIT_FAILURE);
	} else {
		wait(&status);
		if(WEXITSTATUS(status) != 1){
			printf("The comand %s just finished correctly!!!\n\n", exec_args[0]);
			res = 0;
		} else {
			printf("The comand %s not finished correctly!!!\n\n", exec_args[0]);
		}
	}

	free(tofree);

	return res;
}