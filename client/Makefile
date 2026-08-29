CC = gcc
CFLAGS = -Wall -g -DRAYGUI_IMPLEMENTATION
LDFLAGS = -w -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -lenet

.PHONY: all clean

all: main

main: game.c ../sharedLibs/player/player.c ../sharedLibs/strings/strings.c localLibs/network/network.c
	make clean && $(CC) $(CFLAGS) game.c ../sharedLibs/player/player.c ../sharedLibs/strings/strings.c localLibs/network/network.c -o game $(LDFLAGS) && clear && ./game

clean:
	rm -f game && clear