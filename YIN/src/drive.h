#pragma once

#include "main.h"

class Drive {
	private:
		static pros::MotorGroup frontRight;
		static pros::MotorGroup frontLeft;
		static pros::MotorGroup rearRight;
		static pros::MotorGroup rearLeft;
		static pros::IMU tinyBox;
	public:
		Drive();
		static void resetHeading();
		static void setDrivePower(double axF, double axS, double axT);
		static void setDriveVelocity(double speed);
		static void setBrakeMode(pros::motor_brake_mode_e mode);
		static void driveDistance(double distance, double power);
};
