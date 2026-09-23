
srcdir = ../src

SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .c .o

all: main

main: $(srcdir)/main.c
	gcc -Wall -Wextra $(srcdir)/main.c -o main -ggdb

.PHONY: clean
clean:
	rm main