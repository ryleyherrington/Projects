#!/usr/bin/python

import socket
import getopt
import sys
import signal
import time
import re
from socket import gethostname


if (sys.argv[1] == '-help'):
	print ("usage: report.py [-s manage_server] [-N|-R|-C|-K]")
	print ("  -S arg  # connect to 'manage' server on host 'arg'")
	print ("  -N      # report current list of perfect numbers")
	print ("  -R      # report count of numbers searched so far")
	print ("  -C      # report compute clients in use")
	print ("  -K      # kill manage server and all compute clients")
	sys.exit(0)

if (sys.argv[1] == '-S'):
	servername=sys.argv[2]
	nextarg=3
else:
	servername=gethostname()
	nextarg=1

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

#client_socket.connect((str(servername), 2245))
client_socket.connect(('os-class.engr.oregonstate.edu', 2245))
#client_socket.connect(("localhost", 2245))

def sig_int_handler(signum, frame):
	print "Reacting to sig intr"
	client_socket.send('K')
	message = client_socket.recv(1024)
	print message
	sys.exit(0)
signal.signal(signal.SIGINT, sig_int_handler)

def sig_quit_handler(signum, frame):
	print "Reacting to sig quit"
	client_socket.send('K')
	client_socket.recv(1024)
	message = client_socket.recv(1024)
	print message
	sys.exit(0)
signal.signal(signal.SIGQUIT, sig_quit_handler)

def sig_hup_handler(signum, frame):
	print "Reacting to sig hup"
	client_socket.send('K')
	client_socket.recv(1024)
	message = client_socket.recv(1024)
	print message
	sys.exit(0)
signal.signal(signal.SIGHUP, sig_hup_handler)

if (sys.argv[nextarg] == '-s'):
	print "sleeping"
	time.sleep(20)

elif (sys.argv[nextarg] == '-T' and sys.argv[2] is not ''):
	client_socket.send('T '+ str(sys.argv[2]))
	response = client_socket.recv(1024)
	print response

elif (sys.argv[nextarg] == '-N'):
	client_socket.send('N')
	perfect_numbers = client_socket.recv(1024) 
	print ("Perfect numbers found so far: " + str(perfect_numbers))

elif (sys.argv[nextarg] =='-K'):
	client_socket.send('K')
	#response = client_socket.recv(1024)
	print ("Killing server now")
	#print response

elif (sys.argv[nextarg] == '-R'):
	client_socket.send('R ')
	data = client_socket.recv(1024)
	data = data.split(".")[0]
	print ("We have tested "+ (str)(data)+ " numbers.")	
	
elif (sys.argv[nextarg] == '-C'):
	client_socket.send('C')
	comp_list = client_socket.recv(1024)
	print ("Currently running on: " + (str)(comp_list))


client_socket.close()
