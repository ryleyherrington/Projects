#########################################################
#Original Author: Ryley Herrington and D. Kevin McGrath #
#Made February 18, 11:00                                #
#File: Makefile                                         #
#Last Modified: February 22, 2012, 9:00pm, herringr     # 
#                                                       #
#This is the makefile associated with Project 3         #
#########################################################


TARGET=project3
HTML=project3

CCOPTS=-g
CC=g++
CACHE=50000000

default: opt pdf
	sieve 4 4294967295 $(CACHE) noprint
	@echo "now that the timings are done, I'll print out everything"
	sieve 4 4294967295 $(CACHE)

both: pdf html

dvi: ${TARGET}.tex
	latex ${TARGET}.tex
	latex $(TARGET).tex

ps: dvi
	dvips -R -Poutline -t letter ${TARGET}.dvi -o ${TARGET}.ps

pdf: ps
	ps2pdf ${TARGET}.ps


html:
	cp ${TARGET}.tex ${HTML}.tex
	latex ${HTML}.tex
	latex2html -split 0 -show_section_numbers -local_icons -no_navigation ${HTML}
	sed 's/<\/SUP><\/A>/<\/SUP><\/A> /g' < ${HTML}/index.html > ${HTML}/index2.html
	mv ${HTML}/index2.html ${HTML}/index.html
	rm ${HTML}.*

sieve: sieve.c
	$(CC)   $(CCOPTS) -o $@ $@.c -lpthread

opt:
	@rm -f sieve
	make CCOPTS="-O3 -DNDEBUG" sieve

test:	opt
	sieve 4 4294967295 $(CACHE) noprint
	sieve 2 4294967295 $(CACHE) noprint
	sieve 1 4294967295 $(CACHE) noprint

mac:
	make CACHE=10000000 test

clean:
	rm -rf a.out sieve core *.o *.s sieve.tgz sieve.dSYM

tags:
	ctags sieve.c

archive:
	tar cvf - sieve.c timings_graph.eps project3.tex project3.pdf Makefile | bzip2 > sieve.tbz

