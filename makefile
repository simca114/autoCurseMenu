#makefile for zfs snapshots retriever

CC=gcc
DEBG=-g
MESG=-Wall

autoCM: autoCurseMenu.h autoCurseMenu.c
	$(CC) $(DEBG) $(MESG) -c autoCurseMenu.c -lpanel -lncurses

libautoCM: autoCM
	ar -cvq libautoCurseMenu.a autoCurseMenu.o

test: autoCM test.c
	$(CC) $(DEBG) $(MESG) -o test test.c autoCurseMenu.o -lpanel -lncurses

clean:
	rm test autoCurseMenu.o
