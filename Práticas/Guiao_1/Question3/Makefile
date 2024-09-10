CC = gcc
CFLAGS = -Wall

# Com o comando 'Make' compilam o programa
pessoas:
	$(CC) $(CFLAGS) -o pessoas pessoas.c pessoa.c
# Todos os comando deveram ser antecedidos pela palavra make
# Comando para adicionatr um utilizador
run_pessoas_i:
	./pessoas -i "Guilherme Pinho" 13

# Comando para listar os primeiros 3 elementoss
run_pessoas_l:
	./pessoas -l 3

# Comando para atualizar a idade de um utilizador
run_pessoas_u:
	./pessoas -u "Guilherme Pinho" 20

# Comando para adicionar um utilizador devolvendo a posição em que foi adicionado
run_pessoas_i2:
	./pessoas -i2 "Gustavo Santos" 5

# Comando que atualiza a idade de um utilizador através da sua posição na lista
run_pessoas_o:
	./pessoas -o 1 21

# Comando para limpar qualquer ficheiro que seja criado durante a execução do programa
clean: 
	rm -f *.o pessoas file_pessoas lista_pessoas.txt