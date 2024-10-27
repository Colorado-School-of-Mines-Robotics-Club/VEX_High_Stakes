#include "drivetrain.h"
#include "constants.h"

DriveTrain::DriveTrain() :
    FrontRight(FRONT_RIGHT_DRIVE_PORTS),
    FrontLeft(FRONT_LEFT_DRIVE_PORTS),
    RearRight(REAR_RIGHT_DRIVE_PORTS),
    RearLeft(REAR_LEFT_DRIVE_PORTS),
    TinyBox(IMU_PORT)
		{	FrontRight.set_gearing(pros::E_MOTOR_GEAR_200);
			FrontLeft.set_gearing(pros::E_MOTOR_GEAR_200);
			RearRight.set_gearing(pros::E_MOTOR_GEAR_200);
			RearLeft.set_gearing(pros::E_MOTOR_GEAR_200);
			};


void DriveTrain::resetHeading(){
	TinyBox.reset(true);
}

void DriveTrain::setDrivetrainPower(double f, double s, double t){

	double motorPowers [4];
	motorPowers[0] = ((f-t-s)/127.0)*200;
	motorPowers[1] = ((f+t+s)/127.0)*200;
	motorPowers[2] = ((f-t+s)/127.0)*200;
	motorPowers[3] = ((f+t-s)/127.0)*200;
	
	FrontRight.move_velocity(motorPowers[0]);
	FrontLeft.move_velocity(motorPowers[1]);
	RearRight.move_velocity(motorPowers[2]);
	RearLeft.move_velocity(motorPowers[3]);
}

void DriveTrain::setDriveTrainVelocity(double speed){
	FrontRight.move_velocity(speed);
	FrontLeft.move_velocity(speed);
	RearRight.move_velocity(speed);
	RearLeft.move_velocity(speed);
}

void DriveTrain::setBrakeMode(pros::motor_brake_mode_e mode) {
    FrontRight.set_brake_mode(mode);
	FrontLeft.set_brake_mode(mode);
	RearRight.set_brake_mode(mode);
	RearLeft.set_brake_mode(mode);
}


