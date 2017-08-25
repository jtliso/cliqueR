# Makefile for Clique_Enum

CC = gcc
CFLAGS = -O3 -g -Wall
PTHREADS = -lpthread
TESTFLAGS = #-DPERFORMANCE -DDEBUG
LFSFLAG = -D_FILE_OFFSET_BITS=64

OBJECTS = graph.o utility.o bk.o
HEADERS = graph.h utility.h bk.h
EXECUTABLE = clique

all: $(EXECUTABLE)

clique: clique-driver.o $(OBJECTS) $(HEADERS) Makefile
	$(CC) $(CFLAGS) $(LFSFLAG) -o $@ clique-driver.o $(OBJECTS) $(PTHREADS)

.c.o:
	$(CC) $(CFLAGS) $(LFSFLAG) $(TESTFLAGS) -c $*.c

clean:
	rm -f core* *.o $(EXECUTABLE) gmon.out gprof.out

