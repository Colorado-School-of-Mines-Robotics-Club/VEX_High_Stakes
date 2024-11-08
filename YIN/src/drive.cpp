#include "drive.h"
#include "constants.h"

pros::MotorGroup Drive::frontRight(FRONT_RIGHT_DRIVE_PORTS);
pros::MotorGroup Drive::frontLeft(FRONT_LEFT_DRIVE_PORTS);
pros::MotorGroup Drive::rearRight(REAR_RIGHT_DRIVE_PORTS);
pros::MotorGroup Drive::rearLeft(REAR_LEFT_DRIVE_PORTS);
pros::IMU Drive::tinyBox(IMU_PORT);

Drive::Drive() {
	frontRight.set_gearing(pros::E_MOTOR_GEAR_BLUE);
	frontLeft.set_gearing(pros::E_MOTOR_GEAR_BLUE);
	rearRight.set_gearing(pros::E_MOTOR_GEAR_BLUE);
	rearLeft.set_gearing(pros::E_MOTOR_GEAR_BLUE);
}

void Drive::resetHeading(){
	tinyBox.reset(true);
}

void Drive::setDrivePower(double f, double s, double t){

	double motorPowers [4];
	motorPowers[0] = ((f-t-s)/127.0)*200;
	motorPowers[1] = ((f+t+s)/127.0)*200;
	motorPowers[2] = ((f-t+s)/127.0)*200;
	motorPowers[3] = ((f+t-s)/127.0)*200;
	
	frontRight.move_velocity(motorPowers[0]);
	frontLeft.move_velocity(motorPowers[1]);
	rearRight.move_velocity(motorPowers[2]);
	rearLeft.move_velocity(motorPowers[3]);
}

void Drive::setDriveVelocity(double speed){
	frontRight.move_velocity(speed);
	frontLeft.move_velocity(speed);
	rearRight.move_velocity(speed);
	rearLeft.move_velocity(speed);
}

void Drive::setBrakeMode(pros::motor_brake_mode_e mode) {
    frontRight.set_brake_mode(mode);
	frontLeft.set_brake_mode(mode);
	rearRight.set_brake_mode(mode);
	rearLeft.set_brake_mode(mode);
}

void Drive::driveDistance(double distance, double power) {
	frontRight.tare_position();
	frontLeft.tare_position();
	rearRight.tare_position();
	rearLeft.tare_position();
	double target =  distance * DRIVE_MULTIPLIER;
	frontRight.move_relative(target, power * 2);
	frontLeft.move_relative(target, power * 2);
	rearRight.move_relative(target, power * 2);
	rearLeft.move_relative(target, power * 2);
}

