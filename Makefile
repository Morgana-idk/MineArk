CC = gcc
CFLAGS = -Wall -g -DRAYGUI_IMPLEMENTATION
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -lenet

.PHONY: all clean

all: main

main: main.c
	make clean && $(CC) $(CFLAGS) game.c -o game $(LDFLAGS) && clear && ./game

clean:
	rm -f main && clear