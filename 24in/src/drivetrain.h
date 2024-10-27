#ifndef MINES_DRIVE_H
#define MINES_DRIVE_H

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
	pros::MotorGroup FrontRight;
	pros::MotorGroup FrontLeft;
	pros::MotorGroup RearRight;
	pros::MotorGroup RearLeft;
	pros::IMU TinyBox;
};


#endif // MINES_DRIVE_H
