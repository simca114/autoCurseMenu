#makefile for zfs snapshots retriever

CC=gcc
DEBG=-g
MESG=-Wall

libautoCM: autoCM
	rm libautoCurseMenu.a
	ar -cvq libautoCurseMenu.a autoCurseMenu.o

autoCM: autoCurseMenu.h autoCurseMenu.c
	$(CC) $(DEBG) $(MESG) -c autoCurseMenu.c -lpanel -lncurses -lcurses

test: autoCM test.c
	$(CC) $(DEBG) $(MESG) -o test test.c autoCurseMenu.o -lpanel -lncurses


clean:
	rm test autoCurseMenu.o
