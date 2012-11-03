
CPP=g++
CC=gcc

#CCOPTS=-g
CCOPTS=

all: compute

compute: compute.c
	$(CPP) $(CCOPTS) -o $@ $@.c -lpthread

clean:
	rm -f core a.out compute *.o *.s tags
	rm -rf *.dSYM

socket.tgz::
	tar cvf - manage.py report.py compute.c Makefile | gzip > socket.tgz

archive: socket.tgz

