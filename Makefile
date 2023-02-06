CC = gcc
CFLAGS = -g -Wall -std=gnu99

all: pass-gen

pass-gen: pass-gen.c
	$(CC) $(CFLAGS) -o pass-gen pass-gen.c

clean:
	rm -f *.o
	rm -f pass-gen
