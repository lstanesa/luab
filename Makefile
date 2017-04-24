CC=cc
EXE=luab
LDFLAGS=$(shell pkg-config lua52 --libs) -Wall
CFLAGS=$(shell pkg-config lua52 --cflags)

all: clean build

build: luab.o
	$(CC) -o ${EXE} luab.o ${LDFLAGS}

luab.o:
	$(CC) -c luab.c ${CFLAGS}

clean:
	-rm -r *.o luab
