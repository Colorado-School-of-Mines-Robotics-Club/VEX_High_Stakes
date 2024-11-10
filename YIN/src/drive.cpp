#include "drive.h"
#include "constants.h"

pros::MotorGroup Drive::right(RIGHT_DRIVE_PORTS);
pros::MotorGroup Drive::left(LEFT_DRIVE_PORTS);
pros::IMU Drive::tinyBox(IMU_PORT);

Drive::Drive() {
	left.set_gearing(pros::E_MOTOR_GEAR_BLUE);
	right.set_gearing(pros::E_MOTOR_GEAR_BLUE);
}

void Drive::control(double left_power, double right_power) {
	left.move(left_power * DRIVE_SPEED_MULTIPLIER);
	right.move(right_power * DRIVE_SPEED_MULTIPLIER);
}

void Drive::resetHeading(){
	tinyBox.reset(true);
}

void Drive::setDriveVelocity(double speed){
	left.move_velocity(speed);
	right.move_velocity(speed);
}

void Drive::setBrakeMode(pros::motor_brake_mode_e mode) {
	left.set_brake_mode(mode);
    right.set_brake_mode(mode);
}

void Drive::driveDistance(double distance, double power) {
	left.tare_position();
	right.tare_position();
	double target =  distance * DISTANCE_MULTIPLIER;
	right.move_relative(target, power * 2);
	left.move_relative(target, power * 2);
}

