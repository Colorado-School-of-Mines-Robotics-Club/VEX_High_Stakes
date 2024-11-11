#pragma once

// Global
#define MAX_VOLTAGE 127

// Drive 
#define LEFT_DRIVE_PORTS  { -7, -8,  -9,  10}
#define RIGHT_DRIVE_PORTS { 17, -18, 19, 20}
#define IMU_PORT 21
#define DRIVE_SPEED_MULTIPLIER 0.7 
#define PRECISION_MULTIPLIER 0.35
// #define DISTANCE_MULTIPLIER (5350/74.25)
// #define DISTANCE_MULTIPLIER 38.84459628
#define DISTANCE_MULTIPLIER 40.635

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

// Sensors
#define COLOR_SENSOR_TOLERANCE 10 // needs testing
#define COLOR_SENSOR_PORT 42 // CHANGEME

// Unused
// #define FRONT_RIGHT_DRIVE_PORTS { 19,  20}
// #define FRONT_LEFT_DRIVE_PORTS  {-11, -12}
// #define REAR_RIGHT_DRIVE_PORTS  { 17,  18}
// #define REAR_LEFT_DRIVE_PORTS   {-13, -14}