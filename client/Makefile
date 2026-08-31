CC = gcc
CFLAGS = -Wall -g -DRAYGUI_IMPLEMENTATION
LDFLAGS = -w -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -lenet -lncurses
SLIBS = ../sharedLibs
LIBS = $(SLIBS)/player/player.c $(SLIBS)/strings/strings.c $(SLIBS)/render/render.c $(SLIBS)/worldGeneration/generation.c localLibs/network/network.c

.PHONY: all clean

all: main

main: game.c $(LIBS)
	make clean && $(CC) $(CFLAGS) game.c $(LIBS) -o game $(LDFLAGS) && clear && ./game

clean:
	rm -f game && clear