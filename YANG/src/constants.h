#pragma once

// Global
#define MAX_POWER 127
#define POWER_TO_VOLTAGE (12000/127)
#define VOLTAGE_TO_POWER (127/12000)

// Drive 
#define LEFT_DRIVE_PORTS  { -7, -8,  -9,  10}
#define RIGHT_DRIVE_PORTS { 17, -18, 19, 20}
#define IMU_PORT 16
#define DRIVE_SPEED_MULTIPLIER 0.8
#define TURN_SPEED_MULTIPLIER 0.6
#define PRECISION_MULTIPLIER 0.35

// Drive auto
#define DRIVE_UNIT_MULTIPLIER 40.6350
// #define DRIVE_DEG_MULTIPLIER 3.856
#define DRIVE_DEG_MULTIPLIER 0.9

#define DRIVE_TURN_MULTIPLIER 1.00
#define TRACK_WIDTH 12.23

// Intake
#define INTAKE_PORT -4
#define INTAKE_SPEED 127
#define OUTTAKE_SPEED 63

// Conveyor
#define CONVEYOR_PORT 5
#define CONVEYOR_FORWARD_SPEED 127
#define CONVEYOR_REVERSE_SPEED 63

// Goal Grabber
#define GOAL_GRABBER_PORT 'A'

// Arm
#define ARM_PORT 'B'

// Sensors
#define COLOR_SENSOR_TOLERANCE 10 // needs testing
#define COLOR_SENSOR_PORT 42 // CHANGEME

// Unused
// #define FRONT_RIGHT_DRIVE_PORTS { 19,  20}
// #define FRONT_LEFT_DRIVE_PORTS  {-11, -12}
// #define REAR_RIGHT_DRIVE_PORTS  { 17,  18}
// #define REAR_LEFT_DRIVE_PORTS   {-13, -14}