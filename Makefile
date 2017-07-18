CC = gcc
CFLAGS =-g -O3 -Wall -Wextra -pedantic -std=gnu11
LDFLAGS=-lm -lsodium

MKDIR_P = mkdir -p
BINDIR = bin
SOURCEDIR =

.PHONY: clean all

all : minheap graph filehandling bdj main

rebuild : clean all

minheap : minheap.c minheap.h
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -c $< -o $(BINDIR)/$@.o

graph : graph.c graph.h $(BINDIR)/minheap.o
	$(CC) $(CFLAGS) -c $< -o $(BINDIR)/$@.o

filehandling : filehandling.c filehandling.h $(BINDIR)/graph.o
	$(CC) $(CFLAGS) -c $< -o $(BINDIR)/$@.o

bdj : bdj.c bdj.h $(BINDIR)/graph.o $(BINDIR)/minheap.o
	$(CC) $(CFLAGS) -c $< -o $(BINDIR)/$@.o

main : main.c main.h $(BINDIR)/graph.o $(BINDIR)/minheap.o $(BINDIR)/filehandling.o
	$(CC) $(CFLAGS) $< $(BINDIR)/*.o -o $(BINDIR)/$@ $(LDFLAGS)

clean :
	rm -rf $(BINDIR)/*.o $(BINDIR)/main
