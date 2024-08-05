#!/usr/bin/env python
#coding: utf8

import socket

UDP_IP = "192.168.6.86"
UDP_PORT = 1234
MESSAGE = "song=5"

print ("UDP target IP:", UDP_IP)
print ("UDP target port:", UDP_PORT)
print ("message:", MESSAGE)

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(MESSAGE.encode(), (UDP_IP, UDP_PORT))
