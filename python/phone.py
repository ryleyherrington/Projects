#!/usr/bin/python
import urllib2
import re

def phone_numbers():
    layout = re.compile(r'''(\d{3})\D+(\d{3})\D+(\d{4})''', re.VERBOSE)
    #layout = re.compile(r'''(\d{3})[-().](\d{3})[-().](\d{4})''', re.VERBOSE)

    numbers = urllib2.urlopen('http://eecs.oregonstate.edu/people/faculty-directory2').readlines()

    for phonenum in numbers:
        for match in layout.finditer(phonenum):
            if match:
                   area_code, exchange, terminal = match.groups()
                   print area_code, exchange, terminal

phone_numbers()
