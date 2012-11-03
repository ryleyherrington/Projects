#!/usr/bin/python

import math

pcount = 0
candidate = 1

while pcount < 1001:
	candidate = candidate + 1
	fail = 0
	for j in range(2, candidate):
		if candidate % j == 0:
			fail = 1
			break	

	if fail == 0: 
		pcount=pcount+1

	if pcount == 1001:
		print 'The 1001st prime number is ' + str(candidate)

