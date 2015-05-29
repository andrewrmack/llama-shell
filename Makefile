### Makefile for llama-shell ###

OBJS=main.o command.o
LIBS=command.h
PROG=llama-shell

CFLAGS=-O2
INCL=-Iinclude/
CC=gcc

vpath %.h include/
vpath %.c src/

all: $(PROG)

debug: CFLAGS=-g -Wall -Wextra -pedantic
debug: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c $(LIBS)
	$(CC) $(INCL) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o $(PROG)
