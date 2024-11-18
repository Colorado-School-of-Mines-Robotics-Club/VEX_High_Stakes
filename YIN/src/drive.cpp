#include "drive.h"
#include "constants.h"
#include <vector>

pros::MotorGroup Drive::right(RIGHT_DRIVE_PORTS);
pros::MotorGroup Drive::left(LEFT_DRIVE_PORTS);
pros::IMU Drive::tinyBox(IMU_PORT);
pros::Optical Drive::colorSensor(COLOR_SENSOR_PORT);

double abs(double x) {
	if(x < 0) {
		x *= -1;
	}
	return x;
}

double average(std::vector<double> x) {
	double total{0};
	for(double d : x) {
		total += d;
	}
	return total / x.size();
}

double average(std::vector<double> x, std::vector<double> y) {
	double total{0};
	for(double d : x) {
		total += d;
	}
	for(double d : y) {
		total += d;
	}
	return total / (x.size() + y.size());
}


Drive::Drive() {
	left.set_gearing(pros::E_MOTOR_GEAR_BLUE);
	right.set_gearing(pros::E_MOTOR_GEAR_BLUE);
	tinyBox.reset();
}

void Drive::controlTank(double left_power, double right_power, bool precisionButton) {
	if (precisionButton) {
		left.move(left_power * PRECISION_MULTIPLIER);
		right.move(right_power * PRECISION_MULTIPLIER);
	} else {
		left.move(left_power * DRIVE_SPEED_MULTIPLIER);
		right.move(right_power * DRIVE_SPEED_MULTIPLIER);
	}
}

void Drive::controlArcade(double forward_power, double turn_power, bool precisionButton) {
	double left_power = forward_power + turn_power;
	double right_power = forward_power - turn_power;

	if (precisionButton) {
		left.move(left_power * PRECISION_MULTIPLIER);
		right.move(right_power * PRECISION_MULTIPLIER);
	} else {
		left.move(left_power * DRIVE_SPEED_MULTIPLIER);
		right.move(right_power * DRIVE_SPEED_MULTIPLIER);
	}
}

void Drive::resetHeading() {
	tinyBox.reset(true);
}

void Drive::brake() {
	left.brake();
	right.brake();
}

void Drive::setBrakeMode(pros::motor_brake_mode_e mode) {
	left.set_brake_mode(mode);
    right.set_brake_mode(mode);
}

void Drive::setDriveVelocity(int32_t power) {
	left.move(power);
	right.move(power);
}

void Drive::driveDistance(double distance, int32_t power) {
	left.tare_position_all();
	right.tare_position_all();
	double target =  distance * DRIVE_UNIT_MULTIPLIER;
	power = distance > 0 ? power : -power; // Drive backwards if negative distance
	right.move(power);
	left.move(power);
	while(abs(average(left.get_position_all(), right.get_position_all())) < abs(target)) {}
	right.move(0);
	left.move(0);
}

void Drive::driveDistanceGyro(double distance, int32_t power) {
	left.tare_position_all();
	right.tare_position_all();
	tinyBox.tare();
	tinyBox.set_heading(180);

	double target =  distance * DRIVE_UNIT_MULTIPLIER;
	power = distance > 0 ? power : -power; // Drive backwards if negative distance
	
	while(abs(average(left.get_position_all(), right.get_position_all())) < abs(target)) {
		double left_power;
		double right_power;
		double heading = tinyBox.get_heading();
		if(heading == PROS_ERR_F) {
			left_power = power;
			right_power = power;
		} else {
			left_power = power * heading/180;
			right_power = power * abs(180-heading)/180;
		}
		left.move(left_power);
		right.move(right_power);
	}
	right.move(0);
	left.move(0);
}

void Drive::driveDistanceFeedbackBasic(double distance, int32_t minPower, int32_t maxPower) {
	left.tare_position_all();
	right.tare_position_all();

	double target =  distance * DRIVE_UNIT_MULTIPLIER;
	double distance_traveled = 0;
	int dir = distance > 0 ? 1 : -1; // Drive backwards if negative distance
	
	while(distance_traveled < abs(target)) {
		distance_traveled = abs(average(left.get_position_all(), right.get_position_all()));

		double left_power;
		double right_power;
		// Parabola that starts at minPower, maxes at maxPower, ends at minPower
		left_power = dir * (maxPower-minPower) * ((distance_traveled) * (target - distance_traveled)) / (target*target/4) + minPower;
		right_power = dir * (maxPower-minPower) * ((distance_traveled) * (target - distance_traveled)) / (target*target/4) + minPower;
		left.move(left_power);
		right.move(right_power);
	}
	right.move(0);
	left.move(0);
}

void Drive::turn(double deg, int32_t power) {
	left.tare_position_all();
	right.tare_position_all();	
	tinyBox.tare_rotation();
	double cw = (deg > 0) ? 1.0 : -1.0; // Turn cw if deg is positive
	left.move(power * cw);
	right.move(-power * cw);
	while(abs(tinyBox.get_rotation()) < (abs(deg) * DRIVE_DEG_MULTIPLIER)) {}
	left.move(0);
	right.move(0);
}

void Drive::driveArc(double radius, double percentage, double power) {
	double wheel_distance = TRACK_WIDTH / 2;
	double distance = 2 * M_PI * (radius + wheel_distance) * abs(percentage) * DRIVE_UNIT_MULTIPLIER * DRIVE_TURN_MULTIPLIER;

	double left_power{0};
	double right_power{0};
	
	left.tare_position_all();
	right.tare_position_all();

	bool clockwise = percentage > 0;
	if(clockwise) {
		left_power = power;
		right_power = power * ((radius - wheel_distance) / (radius + wheel_distance)); 
		left.move(left_power);
		right.move(right_power);
		while(abs(average(left.get_position_all())) < distance) {}
	} else {
		left_power = power * ((radius - wheel_distance) / (radius + wheel_distance));
		right_power = power;
		left.move(left_power);
		right.move(right_power);
		while(abs(average(right.get_position_all())) < distance) {}
	}
	left.move(0);
	right.move(0);
}

void Drive::driveArcDistance(double radius, double inches, double power) {
	double wheel_distance = TRACK_WIDTH / 2;
	double distance = inches * DRIVE_UNIT_MULTIPLIER * DRIVE_TURN_MULTIPLIER;

	double left_power{0};
	double right_power{0};
	left.tare_position_all();
	right.tare_position_all();

	bool clockwise = distance > 0;
	if(clockwise) {
		left_power = power;
		right_power = power * ((radius - wheel_distance) / (radius + wheel_distance)); 
		left.move(left_power);
		right.move(right_power);
		while(abs(average(left.get_position_all())) < distance) {}
	} else {
		left_power = power * ((radius - wheel_distance) / (radius + wheel_distance));
		right_power = power;
		left.move(left_power);
		right.move(right_power);
		while(abs(average(right.get_position_all())) < distance) {}
	}
	left.move(0);
	right.move(0);
}

void Drive::driveTime(uint32_t milis, int32_t left_power, int32_t right_power) {
	left.move(left_power);
	right.move(right_power);
	pros::delay(milis);
	left.move(0);
	right.move(0);
}

void Drive::driveUntilColor(uint32_t color, int32_t left_power, int32_t right_power) {
	double red = (color & 0x00FF0000) >> 16;
	double green = (color & 0x0000FF00) >> 8;
	double blue = (color & 0x000000FF);
	
	while (true) {
		auto sensedColor = colorSensor.get_rgb();
		if (
			(red - sensedColor.red) < COLOR_SENSOR_TOLERANCE
			&& (green - sensedColor.green) < COLOR_SENSOR_TOLERANCE
			&& (blue - sensedColor.blue) < COLOR_SENSOR_TOLERANCE
		) {
			break;
		}

		left.move(left_power);
		right.move(right_power);
		pros::delay(10);
	}

	left.brake();
	right.brake();
}

void Drive::driveUntilMotorVoltage(double voltage, int32_t left_power, int32_t right_power) {
	while (left.get_voltage() < voltage && right.get_voltage() < voltage) {
		left.move(left_power);
		right.move(right_power);
	}
	
	left.brake();
	right.brake();
}