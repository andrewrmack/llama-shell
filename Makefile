### Makefile for llama-shell ###

OBJ=main.o command.o
PROG=llama-shell

CFLAGS=-O2
CC=gcc

all: $(PROG)

debug: CFLAGS=-g -Wall -Wextra -pedantic
debug: $(PROG)

$(PROG): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.c command.h
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o $(PROG)
