#pragma once

#include "main.h"

class DriveTrain
{
public:
	DriveTrain();
	void resetHeading();
	void setDrivetrainPower(double axF, double axS, double axT);
	void setDriveTrainVelocity(double speed);
	void setBrakeMode(pros::motor_brake_mode_e mode);

private:
	pros::MotorGroup frontRight;
	pros::MotorGroup frontLeft;
	pros::MotorGroup rearRight;
	pros::MotorGroup rearLeft;
	pros::IMU tinyBox;
};
