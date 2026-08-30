CC = gcc
CFLAGS = -Wall -g -lenet -lraylib -lm
SLIBS = ../sharedLibs
LIBS = $(SLIBS)/player/player.c $(SLIBS)/strings/strings.c $(SLIBS)/render/render.c $(SLIBS)/worldGeneration/generation.c

.PHONY: all clean

all:main

main:server.c $(LIBS)
	make clean && $(CC) $(CFLAGS) server.c $(LIBS) -o server && clear && ./server


clean:
	rm -f main && clear