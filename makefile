#makefile for zfs snapshots retriever

CC=gcc
DEBG=-g
MESG=-Wall

libautoCM: autoCM
	rm libautoCurseMenu.a
	ar -cvq libautoCurseMenu.a autoCurseMenu.o

test: autoCM test.c
	$(CC) $(DEBG) $(MESG) -o test test.c autoCurseMenu.o -lpanel -lncurses

autoCM: autoCurseMenu.h autoCurseMenu.c
	$(CC) $(DEBG) $(MESG) -c autoCurseMenu.c -lpanel -lncurses -lcurses

clean:
	rm test autoCurseMenu.o
