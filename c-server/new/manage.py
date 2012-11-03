#!/usr/bin/python
#
# manage.py
#
# Overview
# --------
# This is the controlling process for a multi-process "perfect number" 
# calculator.  # It is composed of 3 parts:
#     manage    - this "server" process
#     report    - the user interface client
#     compute   - the computation engine client
#
# All socket communication between the processes is composed of single 
# letter commands with character string arguments (in order to avoid any 
# endian/byte-swapping issues).
#
#
# Description of Operation
# ------------------------
# The overall data flow is that "manage" will start first and begin 
# accepting commands.
#
# Each "compute" client will register itself with "manage" via an
# "I" command, providing its own machine name, current PID (to
# differentiate multiple "compute" clients on one machine), and a 
# measure of its operations per second.  "manage" will respond with 
# the first range of numbers to test.
# 
# As each range is completed, "compute" will contact "manage" with a
# "P" command to report any perfect numbers it has found, and "manage"
# will respond with a new range to test.
# 
# When any "report" process starts, it will make a request to "manage"
# and receive a single response.
# 
# To support the KILL command, the "manage" server will save the 
# socket to each compute client in a dictionary named K_Socket.
# The client will be expected to monitor this socket for as long
# as it is executing.
#
# Note that in all cases, the protocol is composed of:
#     - a single command from a client to a server
#     - a single response from server to client
#
# Description of Socket Protocol
# ------------------------------
#
# client     command                 server   response
# ------     ----------------------  -------  --------------------------
# report     N                       manage   list-of-perfect-numbers
# report     R                       manage   count-of-numbers-tested
# report     C                       manage   list-of-compute-clients
# report     T                       manage   T [echo test]
# report     K                       manage   K [sends K to all compute clients]
# calculate  I system pid OPS        manage   low high (range to compute)
# calculate  P list-of-perfect-nums  manage   low high (range to compute)
#

import socket
import sys
import os
import signal
import math
from math   import sqrt, floor
from socket import gethostname 

# Default socket connection parameters (over-ride with command line options)
Port = 2245
#Host = "localhost"
Host = gethostname()

# Tables for holding state of each compute client
K_Socket = {}
client_iops_arr =  {}

high_val = 1 
perfect_list = []
tested_numbers = 0

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((Host, Port))
server_socket.listen(5)

def sig_int_handler(signum, frame):
	print "Reacting to sig intr, closing..."
	server_socket.close()	
	sys.exit(0)
signal.signal(signal.SIGINT, sig_int_handler)

def sig_quit_handler(signum, frame):
	print "Reacting to sig quit, closing..."
	server_socket.close()	
	sys.exit(0)
signal.signal(signal.SIGQUIT, sig_quit_handler)

def sig_hup_handler(signum, frame):
	print "Reacting to sig hup, closing..."
	server_socket.close()	
	sys.exit(0)
signal.signal(signal.SIGHUP, sig_hup_handler)


print ("Waiting for client on port " + str(Port))

while True:
	client_socket, addr = server_socket.accept()
	print "I got a connection from ", addr
	data = client_socket.recv(1024)
	if data == "":
		print "no data on socket"
		continue	
	#print data
	command = data.split(' ')

	if (command[0] == 'K'):
		message = "closing server down now..."
		print message
		for i in K_Socket.values():
			i.send("K")
			data1 = i.recv(1024)
			print data1
			i.close()

		pid = os.getpid()
		os.kill(pid, signal.SIGINT)
		
		sys.exit(0)

	elif (command[0] == 'T'):
		print "Received:", data
		client_socket.send(data)

	elif (command[0] == 'N'):
		perfects = ''
		for i in perfect_list:
			perfects += (str(i) + ' ')		
		client_socket.send(perfects)

	elif (command[0] == 'R'):
		data = (str)(tested_numbers)
		client_socket.send(data)	

	elif (command[0] == 'C'):
		compute_servers = ''
		for i in K_Socket.keys():
			compute_servers += (str(i) + ' ')
		client_socket.send(compute_servers)

	elif (command[0] == 'I'):
		cmd, name, pid, iops = command
		#print cmd
		#print name
		#print pid
		#print iops
		#all debug statements ^^	
		# compute new range based on iops and current max number
		low_val = high_val + 1
		high_val = math.floor(math.sqrt((low_val*low_val)+2*(15 * (float)(iops))))
		RANGE = ((str)(low_val) + ' ' + (str)(high_val))
		client_socket.send(RANGE)

		# save the server port on the compute client for the KILL command
		K_Socket[name+'_'+pid] = client_socket 

		# Save this compute clients state for later reporting
		client_iops_arr[name+'_'+pid] = iops
		print ("iops =" + (str)(client_iops_arr))

	elif (command[0] == 'P'):
		arr = []
		arr = command
		n = len(arr)
		for j in arr:
			if j is not 'P':
				perfect_list.append(j)
		#print ("arr =" + (str)(arr))

		low_val = high_val + 1
		high_val = math.floor(math.sqrt((low_val*low_val)+2*(15 * (float)(iops))))
		RANGE = ((str)(low_val) + ' ' + (str)(high_val))
		print ("Sending new range:" + str(RANGE))
		client_socket.send(RANGE)	
		tested_numbers = tested_numbers + (high_val - low_val) 
		client_socket.close()

	else: 
		print "failed" 
		sys.exit(0)
