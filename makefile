CC=gcc

CFLAGS=-std=c99

all: labi

labi:
		$(CC) $(CFLAGS) -o labirint labirint_test.c  
  