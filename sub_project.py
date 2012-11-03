#!/usr/bin/python

import os
from os import makedirs
from os.path import exists
import sys
import getopt
import math
import re
import urllib2
import signal

course = None
term = None
directoryname = None
url = "" 

def sig_int_handler(signum, frame):
	print "Closing now. Thanks for playing."
	sys.exit(0)
signal.signal(signal.SIGINT, sig_int_handler)


def threve():
	print ('Sum of all multiples of 3 or 5 below 1000: ' + str(sum([i for i in range(1000) if i%3==0 or i%5==0])))
	title()

def prime():
	pcount = 0
	candidate = 1
	while pcount < 1001:
		candidate = candidate + 1
		fail = 0
		for i in range(2, candidate):
			if candidate % i == 0:
				fail = 1
				break

		if fail == 0:
			pcount=pcount+1

		if pcount == 1001:
			print 'The 1001st prime number is ' + str(candidate)
		
	title()	


def phone_numbers():
	global url
	layout = re.compile(r'''(\d{3})\D+(\d{3})\D+(\d{4})''', re.VERBOSE)
	try:
		numbers = urllib2.urlopen(url).readlines()
	except:
		print 'Exception'
		print (str(url))
		sys.exit(2)
		#numbers = urllib2.urlopen('http://www.ups.com/content/us/en/contact/index.html?WT.svl=SecNav').readlines()
	for phonenum in numbers:
		for found in layout.finditer(phonenum):
			if found:
					first, second, last = found.groups()
					print first, second, last
	title()


def db():
	path = os.getcwd()
	newpath = ((str)(path) + '/' + (str)(directoryname))
	print "Created new parent directory here: " 
	print (str)(newpath)

	if not os.path.exists(newpath):
		os.makedirs(newpath)
		assert os.path.exists(newpath)

	subdirectories = ['assignments', 'examples', 'exams', 'lecture_notes', 'submissions']

	for sub in subdirectories:
		subdirs = ((str)(newpath) + '/' + (str)(sub))
		print ("subpath" + '_' + (str)(sub) + " = " + (str)(subdirs))

		if not os.path.exists(subdirs):
			os.makedirs(subdirs)
			assert os.path.exists(subdirs)

	print "Done making directories"
	title()


def consecutive():
	bigNum = """
73167176531330624919225119674426574742355349194934
96983520312774506326239578318016984801869478851843
85861560789112949495459501737958331952853208805511
12540698747158523863050715693290963295227443043557
66896648950445244523161731856403098711121722383113
62229893423380308135336276614282806444486645238749
30358907296290491560440772390713810515859307960866
70172427121883998797908792274921901699720888093776
65727333001053367881220235421809751254540594752243
52584907711670556013604839586446706324415722155397
53697817977846174064955149290862569321978468622482
83972241375657056057490261407972968652414535100474
82166370484403199890008895243450658541227588666881
16427171479924442928230863465674813919123162824586
17866458359124566529476545682848912883142607690042
24219022671055626321111109370544217506941658960408
07198403850962455444362981230987879927244284909188
84580156166097919133875499200524063689912560717606
05886116467109405077541002256983155200055935729725
71636269561882670428252483600823257530420752963450
""".replace('\n','')

	print('The biggest number is: ' + str( max([int(bigNum[i])*int(bigNum[i+1])*int(bigNum[i+2])*int(bigNum[i+3])*int(bigNum[i+4]) for i in range(len(bigNum)-5)])))
	
	title()


def title():
	print "What script would you like to try? (Type a single number):"
	print "    (1) 5 Consecutive number answer -- no input needed"
	print "    (2) Database creator -- invoke with sub_proj.py -t [term] -c [course]"
	print "    (3) Find the 1001st prime -- no input needed"
	print "    (4) Phone number finder -- invoke with sub_proj.py -u [url]"
	print "    (5) Sum of all multiples of 3 and 5 below 1000 -- no input needed"
	print "    (6) Done trying all the scripts, I want to exit now "

	print ("url = " + str(url))
	print ("term = " + (str)(term))
	print ("course = " + (str)(course))
	directoryname = (str)(term)+'_'+ (str)(course)
	print ("dn = " + (str)(directoryname))

	response = raw_input(">")
	if response is '1':
		consecutive()
		
	if response is '2':
		db()
			
	if response is '3':
		prime()

	if response is '4':
		phone_numbers()

	if response is '5':
		threve()

	if response is '6':
		pid = os.getpid()
		os.kill(pid, signal.SIGINT)
	
def usage():
	print "-h(or --help), -t(or --term) [term], -c(or--course) [coursename], -u(or --url) [url]"
	sys.exit(0)

def main():
	global url
	global term
	global course
	global directoryname 

	if len(sys.argv) > 1:

		try:
			opts, args = getopt.getopt(sys.argv[1:], "ht:c:u:", ["help","term=", "course=", "url="])
		except getopt.GetoptError, err:
			print str(err)
			sys.exit(2)
		for o, a in opts:
			if o in ("-h", "--help"):
				usage()
				sys.exit(2)
			elif o in ("-t", "--term"):
				term = a
			elif o in ("-c", "--course"):
				course = a
			elif o in ("-u", "--url"):
				url = a
				#url = a
			else:
				usage()
			
		print ("url = " + str(url))
		print ("term = " + (str)(term))
		print ("course = " + (str)(course))
		directoryname = (str)(term)+'_'+ (str)(course)
		print ("dn = " + (str)(directoryname))
		print ''

	print "Is this your first time through the program? "
	print "    (1) Yes -- press 1" 
	print "    (2) No  -- press 2"
	answer = raw_input(">")

	if answer is '1':
		print 'Okay, here are the options you can choose from:' 
		print 'If you want to do all of the projects, invoke with: '
		print '       sub_proj.py -t [term] -c [course] -u [www.exampleurl.com] '
		print 'If you want to do only scripts that involve no'
		print 'parameters, then you can restart with no arguments'
		print 'Would you like to use the example invocation:' 
		print 'sub_project.py -t winter -c cs311 -u http://eecs.oregonstate.edu/people/faculty-directory2' 
		print '    (1) Yes -- Press 1'
		print '    (2) No -- Press 2'
		answer2 = raw_input(">")
		if answer2 is '1':
			url ='http://eecs.oregonstate.edu/people/faculty-directory2'
			term = 'winter'
			course = 'cs311'
			print ("url = " + str(url))
			print ("term = " + (str)(term))
			print ("course = " + (str)(course))
			directoryname = (str)(term)+'_'+ (str)(course)
			print ("dn = " + (str)(directoryname))
			title()
		if answer2 is '2':
			sys.exit(0)

	elif answer is '2': 
		title()

	else:
		print "You should press either 1 or 2 next time."
		sys.exit(0)


if __name__ == "__main__":
    main()
