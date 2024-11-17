#include "drive.h"
#include "intake.h"
#include "conveyor.h"
#include "goal_grabber.h"
#include "constants.h"

void doNothing() {
    pros::lcd::set_text(1, "This does nothing!");
}

void driveForward() {
    pros::lcd::set_text(1, "Driving Forward!");
    Drive::driveDistance(10, 30);
    pros::lcd::clear_line(1);
    pros::lcd::set_text(1, "Finished Driving Forward!");
}

void figureEight(bool isBlue) {
    Drive::setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
    if(isBlue) {
        pros::lcd::set_text(1, "Figure Eight!");
        pros::lcd::set_text(2, "Blue!");
        // These delays are to make each action clear for testing
        pros::delay(2000);
        Drive::driveArc(12, 0.5, 6000);
        pros::delay(2000);
        Drive::driveArc(12, -1, 6000);
        pros::delay(2000);
        Drive::driveArc(12, 0.5, 6000);

        pros::delay(2000);
        Drive::driveArc(12, 0.5, -6000);
        pros::delay(2000);
        Drive::driveArc(12, -1, -6000);
        pros::delay(2000);
        Drive::driveArc(12, 0.5, -6000);
        pros::delay(2000);
    } else {
        pros::lcd::set_text(1, "Figure Eight!");
        pros::lcd::set_text(2, "Red!");
        pros::delay(2000);
        Drive::driveArc(12, 0.5, 6000);
        Drive::driveArc(12, -1, 6000);
        Drive::driveArc(12, 0.5, 6000);

        pros::delay(1);
        Drive::driveArc(12, 0.5, -6000);
        Drive::driveArc(12, -1, -6000);
        Drive::driveArc(12, 0.5, -6000);
    }
    
}

void driveForwardContactLadder() {
    pros::lcd::set_text(1, "This does nothing");
    // TODO: implement
}

void driveForwardRushMogo() {
    pros::lcd::set_text(1, "This does nothing");
    // TODO: implement
}


/*
yin: 
0. load ring near top
0. start facing middle ring pile on opposite side of yang
1. intake ring from first stack (don't run conveyor)
2. load one ring onto field goal
3. turn around and load ring onto mogo
*/
void fullAutoOneYang(bool isBlue) {
    const int32_t driveSpeed = 50;
    const int32_t turnSpeed = 40;
    pros::delay(2000); // TODO: REMOVE IN COMPETITION
    Drive::setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
    Intake::setIntaking();
    Drive::driveDistanceGyro(12.75, driveSpeed * 0.8); // Drive past goal
    pros::delay(250);
    Drive::driveDistanceGyro(-3, driveSpeed * 0.75); // Drive back to post
    Intake::setNotMoving();
    pros::delay(200);
    Drive::turn(-85, turnSpeed); // Turn to face post
    Drive::driveDistanceGyro(-4, driveSpeed); // Drive into wall
    Drive::setDriveVelocity(-40); // Keep driving into wall
    Intake::setIntaking();
    Conveyor::conveyTime(800, CONVEYOR_FORWARD_SPEED);
    Drive::setDriveVelocity(20); // Start driving away
    Conveyor::conveyTime(200, CONVEYOR_FORWARD_SPEED);
    Intake::setNotMoving();
    Drive::driveDistanceGyro(5, driveSpeed);
    Drive::turn(210, turnSpeed); // Turn to face mogo
    Drive::driveDistanceGyro(-12, driveSpeed);
    GoalGrabber::setGrabbing(); // Grab onto mogo
    pros::delay(250);
    Conveyor::conveyTime(800, CONVEYOR_FORWARD_SPEED); // Place ring on mogo
    Drive::setDriveVelocity(0);
    GoalGrabber::setNotGrabbing();
}

void fullAutoOneYin(bool isBlue) {
    const int32_t driveSpeed = 60;
    const int32_t turnSpeed = 40;
    pros::delay(2000); // TODO: REMOVE IN COMPETITION
    Drive::driveDistance(-16, driveSpeed);
    Drive::driveArc(12, 0.125, -driveSpeed);
    Drive::driveDistance(-4, driveSpeed);
    GoalGrabber::setGrabbing();
    Drive::driveDistance(-4, driveSpeed);
    Drive::driveArc(12, 0.125, driveSpeed);
    Drive::driveDistance(24, driveSpeed);
}

// Test autos

void rotateOnce() {
    Drive::resetHeading();
    Drive::setBrakeMode(MOTOR_BRAKE_BRAKE);
    pros::lcd::set_text(1, "Rotating");
    Drive::turn(720, 64);
    pros::lcd::clear_line(1);
    pros::lcd::set_text(1, "Finished Rotating!");
}

void driveCircle(double radius) {
    pros::delay(2);
    pros::lcd::set_text(1, "Circle");
    Drive::driveArc(radius, 2, 6000);
    Drive::setDriveVelocity(0);
    pros::lcd::clear_line(1);
    pros::lcd::set_text(1, "Finished Circle!");
}

void driveDistanceGyro(double distance) {
    pros::lcd::set_text(1, "Going the distance");
    Drive::driveDistanceGyro(distance, 60);
    Drive::setDriveVelocity(0);
    pros::lcd::clear_line(1);
    pros::lcd::set_text(1, "Gone the distance");
}

void testIntake() {
    pros::lcd::set_text(1, "Test Intake!");
    Intake::setIntaking();
    pros::delay(2000);
    Intake::setNotMoving();
    pros::delay(2000);
    Intake::setOuttaking();
    pros::delay(2000);
    Intake::setNotMoving();
}