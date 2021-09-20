CC=gcc

CFLAGS=-std=c99

all: test_const

labi:
	$(CC) $(CFLAGS) -o test_const test_const.c
  