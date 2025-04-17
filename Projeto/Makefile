CC = gcc
CFLAGS = -Wall -g

all: 
	$(CC) $(CFLAGS) dclient.c -o dclient
	$(CC) $(CFLAGS) dserver.c -o dserver	

run_server: 
	./dserver	

run_client_a:
	./dclient -a "Hobbit" "Guilherme; Oliver; Zeca" "1937" "files/file1.txt"

run_client_a2:
	./dclient -a "Livro" "Guilherme; Oliver; Zeca" "1937" "files/file2.txt"

run_client_c:
	./dclient -c "doc1"

run_client_d:
	./dclient -d "doc1"

run_client_l:
	./dclient -l "doc1" "Hello"

run_client_s:
	./dclient -s "Hello" 2

clean:
	rm -f -r *.dSYM fifo* dserver dclient *.o