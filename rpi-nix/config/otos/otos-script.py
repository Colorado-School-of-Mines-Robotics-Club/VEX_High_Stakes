#!/usr/bin/env -S /home/pi/code/venv/bin/python3 -u
import qwiic_otos
import serial
import sys
import time
import struct
import argparse
import socket
from cobs import cobs
from math import sqrt
from typing import Generator
from enum import IntEnum

############ CONSTANTS ############

# V5 Brain
BRAIN_BAUD_RATE = 115200
BRAIN_PORTS = [
	"/dev/serial/by-id/usb-VEX_Robotics__Inc_VEX_Robotics_V5_Brain_-_5CDA4900-if02",
	"/dev/serial/by-id/usb-VEX_Robotics__Inc_VEX_Robotics_V5_Brain_-_B7757A00-if02"
]
BRAIN_TIMEOUT = 1

# Control codes sent from V5 Brain
class Codes(IntEnum):
	KEEPALIVE = 0x00
	RECALIBRATE = 0x01

# Sensor
LINEAR_SCALAR = 1.0
ANGULAR_SCALAR = 1.0
OFFSET_X = 0
OFFSET_Y = 0
OFFSET_ANGLE = 0

############ UTIL FUNCTIONS ############

def make_serial(port) -> serial.Serial():
		serial_connection = serial.Serial()
		serial_connection.baudrate = BRAIN_BAUD_RATE
		serial_connection.port = port
		serial_connection.timeout = BRAIN_TIMEOUT
		return serial_connection

def connect_to_brain() -> serial.Serial:
	print("Attempting to connect to v5 brain...")
	# Open a serial connection for each possible port
	serial_connections = map(make_serial, BRAIN_PORTS)
	i = 0
	while True:
		for serial_connection in serial_connections:
			try:
				serial_connection.open()
				print("Successfully connected to v5 brain!")
				return serial_connection
			except:
				continue

		time.sleep(1)
		if i % 30 == 0:
			print(f"Unable to connect after {i} attempts...")
		i = i + 1

recv_buffer = bytearray()

def recv_data(connection) -> Generator[bytes, None, None]:
	"""
	A generator that given the mode and connection, will return any new
	COBS frames received over the connection. This will only take in
	standard error logs from the Vex V5 brain, as standard output gets
	used at the start to print a pros-themed banner that can't be disabled
	as of the present (though there is an open PR)
	"""
	global recv_buffer

	data = connection.read_all()

	for byte in data:
		if byte != 0x00:
			recv_buffer.append(byte)
		else:
			decoded = cobs.decode(recv_buffer)
			recv_buffer.clear()

			if decoded.startswith(b"serr"):
				yield decoded[4:]
			elif decoded.startswith(b"sout"):
				print("[STDOUT] " + decoded[4:].decode("utf-8"), end="")

def send_data(connection, current_position: qwiic_otos.Pose2D):
	# Pack x, y, and heading as a little-endian c-struct with 3 doubles
	packed = struct.pack("<ddd", current_position.x, current_position.y, current_position.h)
	packed = cobs.encode(packed)

	bytes_written = connection.write(packed)
	zero_written = connection.write(b'\0')

	if bytes_written != len(packed) or zero_written != 1:
		print("WARNING: Wrong amount of bytes written to serial!")

############ MAIN PROGRAM FLOW ############

def main():
	print("Starting main program...")

	# Create instance of device
	optical_device = qwiic_otos.QwiicOTOS()

	# Check if it's connected
	if optical_device.is_connected() == False:
		print("The optical tracking device isn't connected to the system. Please check your connection")
		return

	# Initialize the device
	optical_device.begin()

	print("Calibrating IMU...")

	# Keep flat and stable, calibration should take about a second
	# This calibrates the sensor automatically
	optical_device.calibrateImu()

	# Set manual calibration values for scale & offset
	optical_device.setLinearScalar(1.0)
	optical_device.setAngularScalar(1.0)

	offset = qwiic_otos.Pose2D(OFFSET_X, OFFSET_Y, OFFSET_ANGLE)
	optical_device.setOffset(offset)

	# Reset the tracking algorithm - this resets the position to the origin
	optical_device.resetTracking()

	# Wait for a connection to the brain
	connection = connect_to_brain()
	
	# Wait for the start code
	last_keepalive = None
	print("Waiting for first keepalive...")
	started = False
	while last_keepalive is None:
		for data in recv_data(connection):
			match data[0]:
				case Codes.KEEPALIVE:
					print("First keepalive received!")
					last_keepalive = time.time()
					break
				case Codes.RECALIBRATE:
					optical_device.calibrateImu()
					optical_device.resetTracking()

	# Main program loop
	print("Starting communication loop!")
	while True:
		# Check if we haven't received a keepalive recently and pause if so
		if time.time() - last_keepalive > 2:
			print("Haven't received a keepalive recently, pausing...")
			last_keepalive = None
			while last_keepalive is None:
				for data in recv_data(connection):
					match data[0]:
						case Codes.KEEPALIVE:
							print("New keepalive received, restarting!")
							last_keepalive = time.time()
							break
						case Codes.RECALIBRATE:
							optical_device.calibrateImu()
							optical_device.resetTracking()
		# First check for any codes received
		for data in recv_data(connection):
			match data[0]:
				case Codes.KEEPALIVE:
					last_keepalive = time.time()
				case Codes.RECALIBRATE:
					print("Recalibrate code received, calibrating sensor and zeroing position")
					optical_device.calibrateImu()
					optical_device.resetTracking()
		# Then send data
		try:
			# Get position and write over serial to the brain
			current_position = optical_device.getPosition()
		except socket.error as e:
			print(f"WARNING: Unrecognized socket error thrown:\n{e}")
		except Exception as e:
			if not optical_device.is_connected():
				print("WARNING: Lost connection to optical tracking sensor, waiting for reconnection...")
				while True:
					time.sleep(0.5)
					if optical_device.is_connected():
						print("Reconnected to optical tracking sensor, continuing")
						break
			print(f"WARNING: Unrecognized error thrown:\n{e}")

		# And finally send it to the brain
		try:
			send_data(connection, current_position)
		except serial.SerialTimeoutException:
			print("WARNING: Disconnected from brain, attempting to reconnect")
			connection = connect_to_brain()
		except socket.error as e:
			print(f"WARNING: Unrecognized socket error thrown:\n{e}")
		except Exception as e:
			print(f"WARNING: Unrecognized error thrown:\n{e}")
		
		time.sleep(0.001)

if __name__ == '__main__':
	try:
		while True:
			print("Starting main...")
			main()
	except (KeyboardInterrupt, SystemExit) as exErr:
		print("\nEnding program")
		sys.exit(0)
