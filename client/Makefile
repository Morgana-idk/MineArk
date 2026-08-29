CC = gcc
CFLAGS = -Wall -g -DRAYGUI_IMPLEMENTATION
LDFLAGS = -w -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -lenet -lncurses
LIBS = ../sharedLibs/player/player.c ../sharedLibs/strings/strings.c localLibs/network/network.c localLibs/render/render.c

.PHONY: all clean

all: main

main: game.c $(LIBS)
	make clean && $(CC) $(CFLAGS) game.c $(LIBS) -o game $(LDFLAGS) && clear && ./game

clean:
	rm -f game && clear