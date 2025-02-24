CC = gcc
CFLAGS = -Wall -g

a: a.c
	$(CC) $(CFLAGS) -o a a.c

b: b.c
	$(CC) $(CFLAGS) -o b b.c

run: a
	./a

run_2: b
	./b

clean:
	rm -f *.o a b
