CC=gcc
CFLAGS=-g -std=c17 -Wall

all: store_val read_val

clean:
	rm -vf *.o store_val read_val

mem_alloc.o: mem_alloc.c mem_alloc.h
	$(CC) $(CFLAGS) -DVER=$(VERS) -c $*.c -o $@

ctst.o: ctst.c ctst.h mem_alloc.o
	$(CC) $(CFLAGS) -DVER=$(VERS) -c $*.c -o $@

store_val: store_val.c ctst.o mem_alloc.o
	$(CC) -o $@ $(CFLAGS) $< ctst.o mem_alloc.o

read_val: read_val.c ctst.o mem_alloc.o
	$(CC) -o $@ $(CFLAGS) $< ctst.o mem_alloc.o

