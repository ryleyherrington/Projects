
CFLAGS=-g

rar: rar.c
	$(CC) $(CFLAGS) -std=c99 -o $@ rar.c

test: rar files
	rar -c example.rar afile bfile cfile
	ls
	rar -t example.rar 
	rm afile bfile cfile
	rar -x example.rar afile
	ls
	rm -f afile
	rar -x example.rar 
	ls
	rar -c - afile bfile | gzip > ex.rgz
	ls
	gunzip < ex.rgz | rar -t -

files:
	cp /etc/passwd afile
	cp /etc/hosts bfile
	cp /etc/networks cfile

install: rar
	cp rar ~/bin/rar

clean:
	rm -f rar a.out core afile bfile cfile *.rar
	rm -rf rar.dSYM
	rm -f ex.rgz
