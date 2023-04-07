CC=gcc

CFLAGS=-std=gnu99

all: uint32_sub

labi:
	$(CC) $(CFLAGS) -o uint32_sub uint32_sub.c
  