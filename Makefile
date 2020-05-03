CC = cc
DEBUG = -g
CFLAGS = ${DEBUG} -ansi -pedantic -Wall

SRC = fntfs.c util.c
OBJ = ${SRC:.c=.o}

all: fntfs

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.h

fntfs: ${OBJ}
	${CC} -o $@ ${OBJ} ${CFLAGS}

clean:
	rm *.o
