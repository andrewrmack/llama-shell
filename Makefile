### Makefile for llama-shell ###

PREFIX=/usr/local

MACHINFO="$(shell uname -m)-$(shell uname -i)-$(shell uname -o)"

OBJS=main.o command.o getargs.o
LIBS=command.h getargs.h version.h
PROG=llama-shell

CFLAGS=-O2
CPPFLAGS=-DMACHTYPE='$(MACHINFO)'
CC=gcc

all: $(PROG)

debug: CFLAGS=-g -Wall -Wextra -pedantic
debug: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c $(LIBS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

.PHONY: clean install
clean:
	rm -f *.o $(PROG)

install: $(PROG)
	install -m 0755 $(PROG) $(PREFIX)/bin
