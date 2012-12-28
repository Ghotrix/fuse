CFLAGS=-g -c -D_FILE_OFFSET_BITS=64

all: fuse

fuse: main.o
	gcc main.o -lfuse -o test

main.o: main.c
	gcc ${CFLAGS} main.c

clean:
	rm -rf *.o test
