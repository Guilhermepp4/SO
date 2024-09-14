CC = gcc
CFLAGS = -Wall -g

all: ex3 controlador auxiliares

mysystem.o: mysystem.c mysystem.h

ex3: ex3.o mysystem.o

ex3.o: ex3.c mysystem.h

controlador: controlador.o mysystem.o

controlador.o: controlador.c mysystem.h

controlador_:
	$(CC) $(CFLAGS) controlador.c mysystem.c -o controlador

controlador_run:
	./controlador ./a.out ./b.out ./c.out

auxiliares: auxiliar.c
	$(CC) $(CFLAGS) auxiliar.c -o a.out
	$(CC) $(CFLAGS) auxiliar.c -o b.out
	$(CC) $(CFLAGS) auxiliar.c -o c.out

run:
	./ex3

clean:
	rm -rf *.dSYM *.o ex3 controlador a.out b.out c.out