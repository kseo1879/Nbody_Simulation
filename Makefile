CC=gcc
CFLAGS=-g -std=c11 -Wall -Werror
TARGET=program
.PHONY: clean
all: $(TARGET)

nbody: nbody.c nbody_source.c
	$(CC) $(CFLAGS) $^ -o $@ -lpthread -lm

nbody-gui: nbodygui.c
	$(CC) $(CFLAGS) $^ -o $@ -lpthread -lSDL2 -lSDL2_gfx -lm

test: nbodytest.c nbody_source.c
	$(CC) $(CFLAGS) $^ -o $@ -lpthread -lm

clean:
	rm -f *.o
	rm -f nbody-gui
	rm -f nbody
	rm -f test

#test: nbodytest.c
	#$(CC) $(CFLAGS) $^ -o $@ -lpthread -lcmocka -lm
