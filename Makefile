CC = cc
DEBUG = -g
CFLAGS = ${DEBUG} -ansi -pedantic -Wall

PREFIX = /usr/local

SRC = fntfs.c util.c
OBJ = ${SRC:.c=.o}

all: fntfs

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.h

fntfs: ${OBJ}
	${CC} -o $@ ${OBJ} ${CFLAGS}

clean:
	rm -f fntfs ${OBJ}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f fntfs ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/fntfs

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/fntfs

.PHONY: all clean install uninstall
