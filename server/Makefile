CC = gcc
CFLAGS = -Wall -g -lenet

.PHONY: all clean

all:main

main:server.c
	make clean && $(CC) $(CFLAGS) server.c -o server && clear && ./server


clean:
	rm -f main && clear