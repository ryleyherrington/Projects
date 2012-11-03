#!/usr/bin/python
#This will find out how big of a pyramid
# you can make with a given number of objects.

import math
import sys

def usage():
	print "pyramid.py [how many items you have] "
	print "We'll use a default of 1000 items total for this time around."

def main():
	
	if len(sys.argv)>1:
		high = int(sys.argv[1])
		print ("You have "+ str(high) + " items. Now we'll figure out how large your pyramid can be...")
	else:
		usage()
		high = 1000

	n = 1
	sum_num = 1
	sum_of_bases = 0
	while sum_of_bases <= high:
		sum_num = n*n		
		if (sum_of_bases + sum_num) > high:
			print ("Total number of levels possible: "+ str(n-1))
			print ("Number of items used: "+ str(sum_of_bases))
			print ("Base level is a " + str(n-1)+" by "+ str(n-1)+ " square")
		n += 1
		sum_of_bases += sum_num


if __name__ == "__main__":
	main()

