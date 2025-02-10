# Raspberry PI 0 setup

1. Follow https://github.com/thagrol/Guides/blob/main/ethernetgadget.pdf to get an ssh connection
   1. Ensure you have a static MAC address to not mess with WiFi connections
   2. Ours are currently `f2:5d:47:5e:e5:05` (dev) and `f2:5d:47:5e:e5:06` (host) for RPI 1, `f2:5d:47:5e:e5:07` (dev) and `f2:5d:47:5e:e5:08` (host) for RPI 2.
2. Copy `scripts/interfaces` to `/etc/network/interfaces` on the RPI
3. Copy `scripts/discord-webhook` to `/etc/network/if-up.d` on the RPI, ensure it is executable
3. Copy `scripts/robotics-script.service` to `/etc/systemd/system/robotics-script.service` on the RPI, and enable it with `sudo systemctl daemon-reload && sudo systemctl enable --now robotics-script.service`
4. Run `./push-script.sh` to upload the python code (requires rsync)

# Dev setup

To setup development for python:
1. Create a virtual environment with `python3 -m venv venv`
2. Activate said environment with `. venv/bin/activate`
3. Install dependencies with `pip3 install -r requirements.txt`
4. Done!

# General notes

When the PI powers on (when the robot powers on), it should be completely stable and flat on the ground to ensure accurate calibration. The python script is set to run on boot with an enabled systemd service, so make sure the sensor and LiPO shim is connected on boot. The script also takes an optional `--mode` argument that allows you to specify the mode of connection to the brain. The first (and default) is `serial`, which will attempt to connect over `/dev/ttyACM1`. The second (mainly used for development) is `network` which listens on port 8080 for a connection by `host-bridge.py`, which is run on a computer directly connected to the brain. Lastly, `stdout` can be used to have the script just output all data to the console in a user-friendly format, for debugging if the sensor works properly.

## PROS Serial format

By default PROS outputs stdout and stderr over serial encoded with null-terminated COBS-encoded data. Read all data until you get a null byte, and then cobs-decode everything excluding the null byte. The first 4 chars are the stream identifier (`sout` or `serr`), and the next data is the actual data, such as a single byte or a null-terminated string in the case of a string literal.

## Sensor python notes (taken from example code comments)

### IMU Calibration

The IMU on the OTOS includes a gyroscope and accelerometer, which could
have an offset. Note that as of firmware version 1.0, the calibration
will be lost after a power cycle; the OTOS performs a quick calibration
when it powers up, but it is recommended to perform a more thorough
calibration at the start of all your programs. Note that the sensor must
be completely stationary and flat during calibration! When calling
calibrateImu(), you can specify the number of samples to take and whether
to wait until the calibration is complete. If no parameters are provided,
it will take 255 samples and wait until done; each sample takes about
2.4ms, so about 612ms total

### Scalars

Here we can set the linear and angular scalars, which can compensate for
scaling issues with the sensor measurements. Note that as of firmware
version 1.0, these values will be lost after a power cycle, so you will
need to set them each time you power up the sensor. They can be any value
from 0.872 to 1.127 in increments of 0.001 (0.1%). It is recommended to
first set both scalars to 1.0, then calibrate the angular scalar, then
the linear scalar. To calibrate the angular scalar, spin the robot by
multiple rotations (eg. 10) to get a precise error, then set the scalar
to the inverse of the error. Remember that the angle wraps from -180 to
180 degrees, so for example, if after 10 rotations counterclockwise
(positive rotation), the sensor reports -15 degrees, the required scalar
would be 3600/3585 = 1.004. To calibrate the linear scalar, move the
robot a known distance and measure the error; do this multiple times at
multiple speeds to get an average, then set the linear scalar to the
inverse of the error. For example, if you move the robot 100 inches and
the sensor reports 103 inches, set the linear scalar to 100/103 = 0.971

### Offsets

Assuming you've mounted your sensor to a robot and it's not centered,
you can specify the offset for the sensor relative to the center of the
robot. The units default to inches and degrees, but if you want to use
different units, specify them before setting the offset! Note that as of
firmware version 1.0, these values will be lost after a power cycle, so
you will need to set them each time you power up the sensor. For example,
if the sensor is mounted 5 inches to the left (negative X) and 10 inches
forward (positive Y) of the center of the robot, and mounted 90 degrees
clockwise (negative rotation) from the robot's orientation, the offset
would be {-5, 10, -90}. These can be any value, even the angle can be
tweaked slightly to compensate for imperfect mounting (eg. 1.3 degrees).