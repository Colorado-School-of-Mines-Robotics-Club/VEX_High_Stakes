#pragma once

#include "main.h"

class Drive {
	private:
		static pros::MotorGroup left;
		static pros::MotorGroup right;
		static pros::IMU tinyBox;
		static pros::Optical colorSensor;
	public:
		Drive();
		/**
		 * @brief Driver control using tank drive
		 * 
		 * @param left analog input for left side of robot
		 * @param right analog input for right side of robot
		 * @param precisionButton the digital value of the precision button
		 */
		static void controlTank(double left, double right, bool precisionButton);
		/**
		 * @brief Driver control using arcade drive
		 * 
		 * @param forward analog input for driving straight
		 * @param turn analog input for turning
		 * @param precisionButton the digital value of the precision button
		 */
		static void controlArcade(double forward, double turn, bool precisionButton);
		/**
		 * @brief Resets IMU
		 * 
		 */
		static void resetHeading();
		/**
		 * @brief Set brake mode
		 * 
		 * @param mode (brake, coast, hold)
		 */
		static void setBrakeMode(pros::motor_brake_mode_e mode);
		/**
		 * @brief Set the drive train to a specific power
		 * 
		 * @param power The power to apply to the motors
		 */
		static void setDriveVelocity(int32_t power);
		/**
		 * @brief Drive a distance forward at a certain speed
		 * 
		 * @param distance The distance in inches to drive
		 * @param power The power to apply to the motors while driving
		 */
		static void driveDistance(double distance, int32_t power);
		static void driveDistanceGyro(double distance, int32_t power);
		static void driveDistanceFeedbackBasic(double distance, int32_t minPower, int32_t maxPower);
		/**
		 * @brief Turn degress
		 * 
		 * @param degrees The amount of degrees to turn. Negative is counter clockwise
		 * @param power The power to apply to the motors while driving
		 */
		static void turn(double deg, int32_t power);
		/**
		 * @brief Drive the center of the robot along an arc
		 * 
		 * @param radius The radius of the circle
 		 * @param percentage The percentage (0-1) of the circle to drive. Negative is counter-clockwise
		 * @param power The power in millivolts to apply to the motors while driving. Negative drives backwards along the circle
		 * NOTE: Backwards means it will drive along the same arc as forwards but in the opposite direction.
		 * Clockwise and counter-clockwise refer to the orientation for the forard arc
		 */
		static void driveArc(double radius, double percentage, double power);
		/**
		 * @brief Sets left and right motor speed for a specified amount of miliseconds
		 * 
		 * @param milis The time to drive for
		 * @param left_power The power of the left motors in range [-127, 127]
		 * @param right_power The power of the right motors in range [-127, 127]
		 */
		static void driveTime(uint32_t milis, int32_t left_power, int32_t right_power);
		/**
		 * @brief Sets left and right motor speed an indefinite amount of time until a color is seen
		 * 
		 * @param color The color to watch for
		 * @param left_power The power of the left motors in range [-127, 127]
		 * @param right_power The power of the right motors in range [-127, 127]
		 */
		static void driveUntilColor(uint32_t color, int32_t left_power, int32_t right_power);
		/**
		 * @brief Drive until an increase in motor voltage to detect when we hit something
		 * 
		 * @param voltage The target voltage
		 * @param left_power The power of the left motors in range [-127, 127]
		 * @param right_power The power of the right motors in range [-127, 127]
		 */
		static void driveUntilMotorVoltage(double voltage, int32_t left_power, int32_t right_power);
};
