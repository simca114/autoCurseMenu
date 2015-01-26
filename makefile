#makefile for zfs snapshots retriever

CC=gcc
DEBG=-g
MESG=-Wall

autoCM: autoCurseMenu.h autoCurseMenu.c
	$(CC) $(DEBG) $(MESG) -O -c autoCurseMenu.c -lpanel -lmenu -lncurses

test: autoCM test.c
	$(CC) $(DEBG) $(MESG) -o test test.c autoCurseMenu.o -lpanel -lmenu -lncurses

clean:
	rm test autoCurseMenu.o
