#!/usr/bin/python

import math
import sys
import operator 

num1 = 0
num2 = 0
num3 = 0
num4 = 0
printall = 'n'

def usage():
	print "All #'s must be card numbers- under 14 \nUsage: game.py number1 number2 number3 number4 [printall y/n]"
	sys.exit(2)

def twentyfour(num1, num2, num3, num4, printall):
	evalstring=[]
	ops = ['+','-','/','*']	
	nums = [str(num1), str(num2), str(num3), str(num4)]
	answer = 0
	flag = 0
	for i in ops:
		for j in ops:
			for k in ops:
				for n1 in nums:
					for n2 in nums:
						for n3 in nums:
							for n4 in nums:
								if flag is 1:
									#print "Done"
									sys.exit(2)
								answerstring=str(n1+i+n2+j+n3+k+n4)
								answer=eval(n1+i+n2+j+n3+k+n4)
								if answer is 24:
									if printall is 'n':
										print ("One possible way to get to 24 is: " + str(answerstring))
										flag = 1	
									if printall is 'y':
										print ("All the ways to get to 24 are: " + str(answerstring))

def redo(num1, num2, num3, num4, printall):
	evalstring=[]
	ops = ['+','-','/','*']
    nums = [str(num1), str(num2), str(num3), str(num4)]
    answer = 0
    flag = 0
	for i,j,k in ops:
		for n1,n2,n3,n4 in nums:
			if flag is 1:
				sys.exit(2)
			else: 
				answerstring=str(n1+i+n2+j+n3+k+n4)
				answer=eval(n1+i+n2+j+n3+k+n4)
				if answer is 24:
					if printall is 'n':
						print ("One possible way to get to 24 is: " + str(answerstring))
						flag = 1
					if printall is 'y':
						print ("All the ways to get to 24 are: " + str(answerstring))


def main():

	global num1
 	global num2
	global num3
	global num4

	if len(sys.argv)>1:
		n = tuple(sys.argv[1:])
#		print tuple(n)
	else:
		usage()

	num1 = int(n[0])
 	num2 = int(n[1])
	num3 = int(n[2])
	num4 = int(n[3])
	if num1 > 13:
		usage()
	
	printall = 'n'
	if len(sys.argv)>5:
		printall = str(n[4])
	
#	print ("num1 = "+ str(num1))
#	print ("num2 = "+ str(num2))
#	print ("num3 = "+ str(num3))
#	print ("num4 = "+ str(num4))
#	print ("printall = "+ str(printall))
	
	#twentyfour(num1, num2, num3, num4, printall)
	redo(num1, num2, num3, num4, printall)

if __name__ == "__main__":
	main()
