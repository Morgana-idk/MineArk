CC = gcc
CFLAGS = -Wall -g -lenet

.PHONY: all clean

all:main

main:server.c ../sharedLibs/player/player.c ../sharedLibs/strings/strings.c
	make clean && $(CC) $(CFLAGS) server.c ../sharedLibs/player/player.c ../sharedLibs/strings/strings.c -o server && clear && ./server


clean:
	rm -f main && clear