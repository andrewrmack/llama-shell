### Makefile for AShell ###

OBJ=main.o command.o
PROG=ashell

CFLAGS=-Wall -Wextra -pedantic
CC=gcc

all: $(PROG)

debug: CFLAGS+=-g
debug: ashell

$(PROG): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.c command.h
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o $(PROG)
