CC = gcc
CFLAGS = -g -Wall -std=gnu99 -I/usr/local/opt/openssl@3/include
LDFLAGS= -L/usr/local/opt/openssl@3/lib -lcrypto

all: pass-gen

pass-gen: pass-gen.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o pass-gen pass-gen.c

clean:
	rm -f *.o
	rm -f pass-gen
