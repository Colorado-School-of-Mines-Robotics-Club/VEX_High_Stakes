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


Drive::Drive() {
	left.set_gearing(pros::E_MOTOR_GEAR_BLUE);
	right.set_gearing(pros::E_MOTOR_GEAR_BLUE);
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

void Drive::resetHeading(){
	tinyBox.reset(true);
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
	right.move_relative(target, power);
	left.move_relative(target, power);
}

void Drive::turn(double deg, int32_t power) {
	left.tare_position_all();
	right.tare_position_all();	
	double target = deg * DRIVE_DEG_MULTIPLIER;
	left.move_relative(target, power);
	right.move_relative(-target, power);
}

void Drive::driveArc(double radius, double percentage, double power) {
	double wheel_distance = TRACK_WIDTH / 2;
	double distance = 2 * M_PI * (radius + wheel_distance) * abs(percentage) * DRIVE_UNIT_MULTIPLIER;

	double left_power{0};
	double right_power{0};
	
	left.tare_position_all();
	right.tare_position_all();

	bool clockwise = (percentage > 0);
	if(clockwise) {
		left_power = power;
		right_power = power * ((radius - wheel_distance) / (radius + wheel_distance)); 
		left.move_voltage(power);
		right.move_voltage(power);
		while(clockwise && abs(average(left.get_position_all())) < distance) {}
	} else {
		left_power = power * ((radius - wheel_distance) / (radius + wheel_distance));
		right_power = power;
		left.move_voltage(power);
		right.move_voltage(power);
		while(!clockwise && abs(average(right.get_position_all())) < distance) {}
	}
}

void Drive::driveTime(uint32_t milis, int32_t left_power, int32_t right_power) {
	left.move(left_power);
	left.move(right_power);
	pros::delay(milis);
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