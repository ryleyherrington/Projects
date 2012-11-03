#########################################################
#Original Author: Ryley Herrington and D. Kevin McGrath #
#Made March 15, 11:00								#
#File: Makefile										 #
#													   #
#This is the makefile associated with Project 4		 #
#########################################################


TARGET=proj4
CPP=g++
CC=gcc
CCOPTS=-g

default: compute pdf

dvi: ${TARGET}.tex
	latex ${TARGET}.tex
	latex $(TARGET).tex

ps: dvi
	dvips -R -Poutline -t letter ${TARGET}.dvi -o ${TARGET}.ps

pdf: ps
	ps2pdf ${TARGET}.ps


compute: compute.c
	$(CPP) $(CCOPTS) -o $@ $@.c -lpthread

test: compute
	chmod +x manage.py report.py
	manage.py &
	sleep 3
	compute   &
	sleep 15
	report.py -N
	report.py -R
	report.py -C
	sleep 10
	report.py -N
	report.py -K

clean:
	rm -f core a.out compute *.o *.s tags
	rm -rf *.dSYM

socket.tgz::
	tar cvf - manage.py report.py proj4.tex proj4.pdf sub_project.py compute.c Makefile | bzip2 > final.tbz

archive: socket.tgz

