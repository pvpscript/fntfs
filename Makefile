CC = cc
DEBUG = -g
CFLAGS = ${DEBUG} -ansi -pedantic -Wall

SRC = fntfs.c
OBJ = ${SRC:.c=.o}

all: fntfs

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.h

fntfs: ${OBJ}
	${CC} -o $@ ${OBJ} ${CFLAGS}

#fntfs: fntfs.o
#	$(CC) $(CFLAGS) -o fntfs fntfs.o config.h
#
#fntfs.o: fntfs.c config.h
#	$(CC) $(CFLAGS) -c fntfs.c


clean:
	rm *.o
