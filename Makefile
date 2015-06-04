### Makefile for llama-shell ###

PREFIX=/usr/local

OBJS=main.o command.o
LIBS=command.h
PROG=llama-shell

CFLAGS=-O2
INCL=-Iinclude/
CC=gcc

all: $(PROG)

debug: CFLAGS=-g -Wall -Wextra -pedantic
debug: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c $(LIBS)
	$(CC) $(INCL) $(CFLAGS) -c $<

.PHONY: clean install
clean:
	rm -f *.o $(PROG)

install: $(PROG)
	install -m 0755 $(PROG) $(PREFIX)/bin
