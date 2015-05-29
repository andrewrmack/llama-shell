### Makefile for llama-shell ###

OBJS=main.o command.o signal_handlers.o
LIBS=command.h signal_handlers.h
PROG=llama-shell

CFLAGS=-O2
CC=gcc

all: $(PROG)

debug: CFLAGS=-g -Wall -Wextra -pedantic
debug: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c $(LIBS)
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o $(PROG)
