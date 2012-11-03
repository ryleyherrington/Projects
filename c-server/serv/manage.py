#!/usr/bin/python

import socket
import sys
import os
import signal
import math
from math import sqrt, floor

Port = 2245
Host = "localhost"
K_Port = {}

high_val = 1 
perfect_list = []

client_socket_arr =  {}
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
	print data
	command = data.split(' ')

	if (command[0] == 'K'):
		message = "closing server down now..."
		print message
		for i in K_Port.keys():
			print i
			serv_name = i.split("_")[0]
			
			port = int(K_Port[i])

			print("client_socket used by K command: server=" + str(serv_name) + " port=" + str(port))

			client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			client_socket.connect((serv_name, port))

			client_socket.send(message)
			data1 = client_socket.recv(1024)
			print data1
			client_socket.close()
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
		for i in client_socket_arr.keys():
			compute_servers += (str(i) + ' ')
		client_socket.send(compute_servers)

	elif (command[0] == 'I'):
		cmd, name, pid, kill_port, iops = command
		print cmd
		print name
		print pid
		print kill_port
		print iops
		
		
		K_Port[name+'_'+pid] = kill_port
		# compute new range based on iops and current max number
		low_val = high_val + 1
		high_val = math.floor(math.sqrt((low_val*low_val)+2*(15 * (float)(iops))))
		RANGE = ((str)(low_val) + ' ' + (str)(high_val))
		client_socket.send(RANGE)
		client_socket.close()

		# Save this compute clients state for later reporting
		client_range_arr =  {}
		client_iops_arr =  {}
		client_range_arr[name+'_'+pid] = [low_val, high_val]
		client_socket_arr[name+'_'+pid] = client_socket
		print ("client_socket saved by I command for key " + name+'_'+pid + (str)(client_socket))
		client_iops_arr[name+'_'+pid] = iops
                print ("range =" + (str)(client_range_arr))
                print ("socket =" + (str)(client_socket_arr))
                print ("iops =" + (str)(client_iops_arr))

	elif (command[0] == 'P'):
		arr = []
		arr = command
		n = len(arr)
		for j in arr:
			if j is not 'P':
				perfect_list.append(j)
                print ("arr =" + (str)(arr))

		low_val = high_val + 1
		high_val = math.floor(math.sqrt((low_val*low_val)+2*(15 * (float)(iops))))
		RANGE = ((str)(low_val) + ' ' + (str)(high_val))
		print RANGE
		client_socket.send(RANGE)	
		tested_numbers = tested_numbers + (high_val - low_val) 
		client_socket.close()

	else: 
		print "failed" 
		sys.exit(0)
