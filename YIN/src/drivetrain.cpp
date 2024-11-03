#include "drivetrain.h"
#include "constants.h"

DriveTrain::DriveTrain() :
    frontRight(FRONT_RIGHT_DRIVE_PORTS),
    frontLeft(FRONT_LEFT_DRIVE_PORTS),
    rearRight(REAR_RIGHT_DRIVE_PORTS),
    rearLeft(REAR_LEFT_DRIVE_PORTS),
    tinyBox(IMU_PORT)
		{	frontRight.set_gearing(pros::E_MOTOR_GEAR_BLUE);
			frontLeft.set_gearing(pros::E_MOTOR_GEAR_BLUE);
			rearRight.set_gearing(pros::E_MOTOR_GEAR_BLUE);
			rearLeft.set_gearing(pros::E_MOTOR_GEAR_BLUE);
			};


void DriveTrain::resetHeading(){
	tinyBox.reset(true);
}

void DriveTrain::setDrivetrainPower(double f, double s, double t){

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

void DriveTrain::setDriveTrainVelocity(double speed){
	frontRight.move_velocity(speed);
	frontLeft.move_velocity(speed);
	rearRight.move_velocity(speed);
	rearLeft.move_velocity(speed);
}

void DriveTrain::setBrakeMode(pros::motor_brake_mode_e mode) {
    frontRight.set_brake_mode(mode);
	frontLeft.set_brake_mode(mode);
	rearRight.set_brake_mode(mode);
	rearLeft.set_brake_mode(mode);
}


