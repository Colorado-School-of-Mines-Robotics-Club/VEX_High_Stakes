#pragma once

#include "main.h"

class Drive {
	private:
		static pros::MotorGroup left;
		static pros::MotorGroup right;
		static pros::IMU tinyBox;
	public:
		Drive();
		static void control(double left, double right);
		static void resetHeading();
		static void setDriveVelocity(double speed);
		static void setBrakeMode(pros::motor_brake_mode_e mode);
		static void driveDistance(double distance, double power);
};
