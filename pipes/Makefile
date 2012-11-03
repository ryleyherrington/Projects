#########################################################
#Original Author: Ryley Herrington and D. Kevin McGrath #
#Made January 30, 11:00					#
#File: Makefile						#
#Last Modified: January 31, 2012, 9:00pm, herringr	# 
#							#
#This is the makefile associated with Project 2		#
#########################################################


TARGET=template
HTML=main_html

# CC_OPTIONS=-Wall -std=c99
# CC=cc

# CC_OPTIONS=-O
CC_OPTIONS=-g
CC=g++

default: uniqify suppressor

both: pdf html

dvi: ${TARGET}.tex 
#	run latex twice to get references correct
	latex ${TARGET}.tex
#	you can also have a bibtex line here
#	bibtex $(TARGET).tex
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

uniqify: uniqify.cpp
	$(CC) $(CC_OPTIONS) -o $@ $@.cpp

suppressor: suppressor.cpp
	$(CC) $(CC_OPTIONS) -o $@ $@.cpp

run: default
	uniqify 2 < test2.txt

test: uniqify 
	-time uniqify test1.txt 2>&1 | grep real
	-time uniqify test2.txt 2>&1 | grep real
	-time uniqify test3.txt 2>&1 | grep real
	-time uniqify test4.txt 2>&1 | grep real
	-time uniqify test5.txt 2>&1 | grep real
	-time uniqify test6.txt 2>&1 | grep real
	-time uniqify test7.txt 2>&1 | grep real

clean:
	rm -f uniqify *.o core a.out  suppressor uniqify.tgz
	rm -rf *.dSYM

archive:
	tar cvf - Makefile uniqify.cpp suppressor.cpp *.eps *.tex | bzip2 > uniqify.tbz

