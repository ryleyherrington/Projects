#!/usr/bin/python

import os 
from os import makedirs
from os.path import exists
import sys
import getopt

course = None
term = None
directoryname = None

def usage():
	print "-h(or --help), -t(or --term) [term], -c(or--course) [coursename]"
	sys.exit(0)

def main():
	try:
		opts, args = getopt.getopt(sys.argv[1:], "ht:c:", ["help","term=", "course="])
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
		else:
			print "Try again, opts aren't correct"

	print ("term = " + (str)(term))
	print ("course = " + (str)(course))
	directoryname = (str)(term)+'_'+ (str)(course)
	print ("dn = " + (str)(directoryname))

	path = os.getcwd()
	newpath = ((str)(path) + '/' + (str)(directoryname))
	print ("newpath = " + (str)(newpath))

	if not os.path.exists(newpath):
		os.makedirs(newpath)
		assert os.path.exists(newpath)

	subdirectories = ['assignments', 'examples', 'exams', 'lecture_notes', 'submissions']

	for sub in subdirectories:
		subdirs = ((str)(newpath) + '/' + (str)(sub))
		print ("subpath"+(str)(sub)+" = " + (str)(subdirs))
		
		if not os.path.exists(subdirs):
			os.makedirs(subdirs)
			assert os.path.exists(subdirs)

	print "done"
	


if __name__ == "__main__":
	main()

