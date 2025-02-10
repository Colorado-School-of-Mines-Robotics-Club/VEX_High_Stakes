import argparse
from cobs import cobs
import serial
import socket
import struct

# This file allows you to use a local computer to proxy from the RPI to the vex brain
# It should be run with netcat piped in to this script's stdin, like for example:
# `nc 10.0.0.101 8080 | python3 host-proxy.py`

if __name__ == "__main__":
	parser = argparse.ArgumentParser()
	parser.add_argument(
		"address",
		type=str,
		default="10.0.0.101",
		help="The address to connect to"
	)
	parser.add_argument(
		"--tty",
		type=str,
		default="/dev/serial/by-id/usb-VEX_Robotics__Inc_VEX_Robotics_V5_Brain_-_5CDA4900-if02",
		help="The tty device port to connect to (default /dev/serial/by-id/usb-VEX_Robotics__Inc_VEX_Robotics_V5_Brain_-_5CDA4900-if02)"
	)
	args = parser.parse_args()

	ser = serial.Serial(args.tty, baudrate=115200, timeout=0, exclusive=True)
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect((args.address, 8080))
	cobs_buffer = b''
	while True:
		# Proxy network -> serial
		try:
			data = s.recv(1024, socket.MSG_DONTWAIT)
			ser.write(data)
		except socket.error as e:
			if socket.errno.EAGAIN == e.errno:
				pass
			else:
				raise e
		# Proxy serial -> network
		data = ser.read_all()
		if len(data) != 0:
			s.send(data)
