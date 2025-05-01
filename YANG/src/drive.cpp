#include "drive.h"
#include "constants.h"
#include <vector>

DriveDirection Drive::direction(DriveDirection::FACING_INTAKE);

// TODO: fix later drive groups do not actually refer to the motors, but are separately declared from ports
pros::Motor Drive::left_motor_0(LEFT_PORT_0);
pros::Motor Drive::left_motor_1(LEFT_PORT_1);
pros::Motor Drive::left_motor_2(LEFT_PORT_2);
pros::Motor Drive::left_motor_3(LEFT_PORT_3);

pros::Motor Drive::right_motor_0(RIGHT_PORT_0);
pros::Motor Drive::right_motor_1(RIGHT_PORT_1);
pros::Motor Drive::right_motor_2(RIGHT_PORT_2);
pros::Motor Drive::right_motor_3(RIGHT_PORT_3);

pros::MotorGroup Drive::left(LEFT_DRIVE_PORTS);
pros::MotorGroup Drive::right(RIGHT_DRIVE_PORTS);
pros::IMU Drive::tinyBox(IMU_PORT);

double Drive::m_target{0};

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

void Drive::setDistance(double distance) {
	left.tare_position_all();
	right.tare_position_all();
	m_target =  distance * DRIVE_UNIT_MULTIPLIER;
}

bool Drive::atTarget() {
	return abs(average(left.get_position_all(), right.get_position_all())) > abs(m_target);
}

void Drive::move(double left_speed, double right_speed) {
	left.move(left_speed * LEFT_MULTIPLIER);
	right.move(right_speed * RIGHT_MULTIPLIER);
}

void Drive::driveDirect(double left_speeds[4], double right_speeds[4]) {
	left_motor_0.move_velocity(left_speeds[0]);
	left_motor_1.move_velocity(left_speeds[1]);
	left_motor_2.move_velocity(left_speeds[2]);
	left_motor_3.move_velocity(left_speeds[3]);

	right_motor_0.move_velocity(right_speeds[0]);
	right_motor_1.move_velocity(right_speeds[1]);
	right_motor_2.move_velocity(right_speeds[2]);
	right_motor_3.move_velocity(right_speeds[3]);
}


void Drive::controlTank(double left_power, double right_power, bool precisionButton) {
	if (precisionButton) {
		Drive::move(left_power * PRECISION_MULTIPLIER, right_power * PRECISION_MULTIPLIER);
	} else {
		Drive::move(left_power * DRIVE_SPEED_MULTIPLIER, right_power * DRIVE_SPEED_MULTIPLIER);
	}
	Drive::move(left_power, right_power);
}

void Drive::controlArcade(double forward_power, double turn_power, bool precisionButton) {
	double left_power = static_cast<int>(direction) * forward_power + (turn_power * TURN_SPEED_MULTIPLIER);
	double right_power = static_cast<int>(direction) * forward_power - (turn_power * TURN_SPEED_MULTIPLIER);

	if (precisionButton) {
		Drive::move(left_power * PRECISION_MULTIPLIER, right_power * PRECISION_MULTIPLIER);
	} else {
		Drive::move(left_power * DRIVE_SPEED_MULTIPLIER, right_power * DRIVE_SPEED_MULTIPLIER);
	}
}

void Drive::controlDirection(bool directionButton) {
	if(directionButton && direction == DriveDirection::FACING_INTAKE) {
		setFacingGrabber();
	} else if (directionButton && direction == DriveDirection::FACING_GRABBER) {
		setFacingIntake();
	}
}

void Drive::setFacingIntake() {
	direction = DriveDirection::FACING_INTAKE;
}

void Drive::setFacingGrabber() {
	direction = DriveDirection::FACING_GRABBER;
}

void Drive::resetHeading() {
	tinyBox.reset(true);
}

double Drive::getYaw() {
	return tinyBox.get_yaw();
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
	Drive::move(power, power);
}

void Drive::driveDistance(double distance, int32_t power) {
	left.tare_position_all();
	right.tare_position_all();
	double target =  distance * DRIVE_UNIT_MULTIPLIER;
	power = distance > 0 ? power : -power; // Drive backwards if negative distance
	Drive::move(power, power);
	while(abs(average(left.get_position_all(), right.get_position_all())) < abs(target)) {}
	Drive::move(0, 0);
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
		Drive::move(left_power, right_power);
	}
	Drive::move(0, 0);
}

void Drive::driveDistanceFeedbackBasic(double distance, int32_t minPower, int32_t maxPower) {
	left.tare_position_all();
	right.tare_position_all();

	double target =  abs(distance * DRIVE_UNIT_MULTIPLIER);
	double distance_traveled = 0;
	int dir = distance > 0 ? 1 : -1; // Drive backwards if negative distance

	while(distance_traveled < target) {
		distance_traveled = abs(average(left.get_position_all(), right.get_position_all()));

		double left_power;
		double right_power;
		// Parabola that starts at minPower, maxes at maxPower, ends at minPower
		left_power = dir * (maxPower-minPower) * ((distance_traveled) * (target - distance_traveled)) / (target*target/4) + minPower;
		right_power = dir * (maxPower-minPower) * ((distance_traveled) * (target - distance_traveled)) / (target*target/4) + minPower;
		Drive::move(left_power, right_power);
	}
	Drive::move(0, 0);
}

void Drive::turn(double deg, int32_t power) {
	left.tare_position_all();
	right.tare_position_all();
	tinyBox.tare_rotation();
	double cw = (deg > 0) ? 1.0 : -1.0; // Turn cw if deg is positive
	Drive::move(power * cw, -power * cw);
	while(abs(tinyBox.get_rotation()) < (abs(deg) * DRIVE_DEG_MULTIPLIER)) {}
	Drive::brake();
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
		Drive::move(left_power, right_power);
		while(abs(average(left.get_position_all())) < distance) {}
	} else {
		left_power = power * ((radius - wheel_distance) / (radius + wheel_distance));
		right_power = power;
		Drive::move(left_power, right_power);
		while(abs(average(right.get_position_all())) < distance) {}
	}
	Drive::move(0, 0);
}

void Drive::driveArcDistance(double radius, double inches, double power) {
	double wheel_distance = TRACK_WIDTH / 2;
	double distance = abs(inches) * DRIVE_UNIT_MULTIPLIER * DRIVE_TURN_MULTIPLIER;

	double left_power{0};
	double right_power{0};
	left.tare_position_all();
	right.tare_position_all();

	bool clockwise = inches > 0;
	if(clockwise) {
		left_power = power;
		right_power = power * ((radius - wheel_distance) / (radius + wheel_distance));
		Drive::move(left_power, right_power);
		while(abs(average(left.get_position_all())) < distance) {}
	} else {
		left_power = power * ((radius - wheel_distance) / (radius + wheel_distance));
		right_power = power;
		Drive::move(left_power, right_power);
		while(abs(average(right.get_position_all())) < distance) {}
	}
	Drive::move(0, 0);
}

void Drive::driveTime(uint32_t milis, int32_t left_power, int32_t right_power) {
	Drive::move(left_power, right_power);
	pros::delay(milis);
	Drive::move(0, 0);
}

// void Drive::driveUntilColor(uint32_t color, int32_t left_power, int32_t right_power) {
// 	double red = (color & 0x00FF0000) >> 16;
// 	double green = (color & 0x0000FF00) >> 8;
// 	double blue = (color & 0x000000FF);
	
// 	while (true) {
// 		auto sensedColor = colorSensor.get_rgb();
// 		if (
// 			(red - sensedColor.red) < COLOR_SENSOR_TOLERANCE
// 			&& (green - sensedColor.green) < COLOR_SENSOR_TOLERANCE
// 			&& (blue - sensedColor.blue) < COLOR_SENSOR_TOLERANCE
// 		) {
// 			break;
// 		}

// 		Drive::move(left_power, right_power);
// 		pros::delay(10);
// 	}

// 	left.brake();
// 	right.brake();
// }

void Drive::driveUntilMotorVoltage(double voltage, int32_t left_power, int32_t right_power) {
	while (left.get_voltage() < voltage && right.get_voltage() < voltage) {
		Drive::move(left_power, right_power);
	}
	
	left.brake();
	right.brake();
}