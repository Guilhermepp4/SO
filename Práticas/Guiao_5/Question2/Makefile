CC = gcc
CFLAGS = -Wall -g

all: 
	$(CC) $(CFLAGS) client.c -o client
	$(CC) $(CFLAGS) server.c vector.c -o server	

run_all:
	./server
	./client 3

run_server: 
	./server	

run_client:
	./client 3

clean:
	rm -f -r *.dSYM fifo* server client *.o