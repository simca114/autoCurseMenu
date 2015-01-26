#makefile for zfs snapshots retriever

CC=gcc
DEBG=-g
MESG=-Wall

snapshot: test.c
	$(CC) $(DEBG) $(MESG) -o test test.c -lpanel -lmenu -lncurses

clean:
	rm test
