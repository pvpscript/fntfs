CC = cc
DEBUG = -g
CFLAGS = $(DEBUG) -ansi -pedantic -Wall

fntfs: main.o fntfs.o
	$(CC) $(CFLAGS) -o fntfs main.o fntfs.o


main.o: main.c main.h
	$(CC) $(CFLAGS) -c main.c

fntfs.o: fntfs.c fntfs.h defs.h
	$(CC) $(CFLAGS) -c fntfs.c


clean:
	rm *.o
