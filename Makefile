### Makefile for llama-shell ###

PREFIX=/usr/local

MACHINFO="$(shell uname -m)-$(shell uname -i)-$(shell uname -o)"

OBJS=main.o command.o getargs.o
LIBS=command.h getargs.h version.h
PROG=llama-shell

CFLAGS=-O2 -pipe -fstack-protector -Wl,-z,relro -Wl,-z,now -Wl,-O1 \
       -fno-exceptions
CPPFLAGS=-DMACHTYPE='$(MACHINFO)'
CC=gcc

all: $(PROG)

debug: CFLAGS+=-g -Wall -Wextra -pedantic -Wfloat-equal -Wundef \
               -Wshadow -Wpointer-arith -Wcast-align \
               -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings \
               -Waggregate-return -Wcast-qual -Wswitch-default \
               -Wswitch-enum -Wconversion -Wunreachable-code -Werror \
               -Wformat-security -Wformat-nonliteral
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
