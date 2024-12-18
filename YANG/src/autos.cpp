#include "drive.h"
#include "intake.h"
#include "conveyor.h"
#include "arm.h"
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

// broken since driveArc changed
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

// start negative side, rush goal
void fullAutoOneYin(bool isBlue) {
    const int32_t rushSpeed = 90;
    const int32_t driveSpeed = 50;
    const int32_t slowSpeed = 20;
    const int32_t turnSpeed = 50;
    Drive::resetHeading();
    Drive::setBrakeMode(MOTOR_BRAKE_BRAKE);

    if(isBlue) {

    } else {
        pros::delay(2000); // TODO: REMOVE IN COMPETITION
        GoalGrabber::setNotGrabbing();
        Arm::setArmUp();
        Drive::driveArc(TRACK_WIDTH/2, -0.06, rushSpeed); // Align with mogo
        Drive::driveDistanceFeedbackBasic(41, slowSpeed, rushSpeed); // Move to mogo
        Drive::driveDistance(5, slowSpeed); // Approach slowly
        Arm::setArmDown(); // Grab
        pros::delay(250); 
        Drive::driveDistance(-12, rushSpeed); // Run away
        Arm::setArmUp(); // Leave goal
        Drive::turn(-200, turnSpeed); // Turn around
        Drive::driveDistance(-10, driveSpeed); // Align goal
        GoalGrabber::setGrabbing(); // Pickup goal
        Drive::driveDistance(-2, driveSpeed); // Move a little more
        Drive::driveArcDistance(18, 30, rushSpeed); // Drive to ring
        Intake::setIntaking(); // Pickup ring
        Drive::driveArcDistance(18, 8, driveSpeed); // Keep Driving
        Conveyor::conveyTime(2000, CONVEYOR_FORWARD_SPEED); // Put ring on goal
        Intake::setNotMoving();
        pros::delay(200);
        GoalGrabber::setNotGrabbing();
    }
}

/*
yang: 
0. load ring near top
0. start facing middle ring pile on opposite side of yang
1. intake ring from first stack (don't run conveyor)
2. load one ring onto field goal
3. turn around and load ring onto mogo
*/
void fullAutoOneYang(bool isBlue) {
    const int32_t rushSpeed = 90;
    const int32_t fastSpeed = 70;
    const int32_t driveSpeed = 50;
    const int32_t slowSpeed = 30;
    const int32_t turnSpeed = 40;
    Drive::setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
    pros::delay(2000); // TODO: REMOVE IN COMPETITION
    if(isBlue) {
        Intake::setIntaking();
        Drive::driveDistance(15.25, driveSpeed); // Drive past goal
        pros::delay(250);
        Drive::driveDistance(-3, driveSpeed); // Drive back to post 
        pros::delay(200);
        Intake::setNotMoving();
        Drive::turn(-85, turnSpeed); // Turn to face post
        Drive::driveDistance(6, driveSpeed);
        Drive::driveDistance(-12, driveSpeed); // Drive into wall
        Drive::setDriveVelocity(-driveSpeed); // Keep driving into wall
        Intake::setIntaking();
        Conveyor::conveyTime(500, CONVEYOR_FORWARD_SPEED);
        Drive::setDriveVelocity(20); // Start driving away
        Conveyor::conveyTime(200, CONVEYOR_FORWARD_SPEED);
        Intake::setNotMoving();
        Drive::driveDistance(1.5, driveSpeed);
        Drive::turn(175, turnSpeed); // Turn to have pickup facing mogo
        Drive::driveTime(600, -driveSpeed, -driveSpeed);
        GoalGrabber::setGrabbing(); // Grab onto mogo
        pros::delay(250);
        Conveyor::conveyTime(1200, CONVEYOR_FORWARD_SPEED); // Place ring on mogo
        Drive::driveArc(8, 0.225, driveSpeed);
        Drive::driveDistance(25, driveSpeed);
        Intake::setIntaking();
        Drive::driveDistance(12, driveSpeed);
        Conveyor::conveyTime(1500, CONVEYOR_FORWARD_SPEED);
        Intake::setNotMoving();
        Arm::setArmDown();
        Drive::driveDistance(-14, driveSpeed); // Drive away after picking up rings
        Drive::turn(-20, turnSpeed);
        Drive::driveTime(1000, fastSpeed, fastSpeed*0.8);
        Drive::turn(120, driveSpeed);
        GoalGrabber::setNotGrabbing();
        Intake::setNotMoving();
        Conveyor::setNotMoving();
        Drive::turn(60, driveSpeed);  
        Drive::driveTime(1000, -driveSpeed, -driveSpeed);
        Arm::setArmUp();
        pros::delay(250);

        Drive::driveDistance(30, driveSpeed);
        Drive::driveArc(12, 0.125, driveSpeed);
        Drive::driveTime(1000, driveSpeed, driveSpeed);
    } else {
        GoalGrabber::setNotGrabbing();
        Arm::setArmUp();
        Drive::driveArc(TRACK_WIDTH/2, -0.06, rushSpeed); // Align with mogo
        Drive::driveDistanceFeedbackBasic(36, slowSpeed, rushSpeed); // Move to mogo
        Drive::driveDistance(5, slowSpeed);
        Drive::driveArc(TRACK_WIDTH/2, 0.06, slowSpeed);
        Arm::setArmDown(); // Grab
        pros::delay(350); 
        Drive::driveDistance(-2, driveSpeed); // Run away
        Drive::driveDistance(-14, rushSpeed); // Run away
        Drive::driveArc(TRACK_WIDTH/2, 0.06, -slowSpeed);
        Arm::setArmUp(); // Leave goal
        pros::delay(350);
        Drive::driveDistance(-4, driveSpeed);
        Drive::turn(-180, turnSpeed); // Turn around
        Drive::driveDistance(-12, driveSpeed); // Align goal
        Drive::driveDistance(-8, slowSpeed); 
        GoalGrabber::setGrabbing(); // Pickup goal
        Drive::driveDistance(-2, slowSpeed); // Move a little more
        Drive::driveArcDistance(24, 25, rushSpeed); // Drive to ring
        Intake::setIntaking(); // Pickup ring
        Drive::driveArcDistance(24, 15, driveSpeed); // Keep Driving
        Conveyor::conveyTime(1500, CONVEYOR_FORWARD_SPEED); // Put ring on goal
        Intake::setNotMoving();
        GoalGrabber::setNotGrabbing();

        Drive::turn(180, turnSpeed);
        Arm::setArmDown();
        Drive::driveTime(2000, slowSpeed, slowSpeed*0.8);
    }

}

// Test autos
void rushWithArm() {
    const int32_t rushSpeed = 90;
    const int32_t driveSpeed = 50;
    const int32_t slowSpeed = 20;
    const int32_t turnSpeed = 50;
    Drive::resetHeading();
    pros::delay(2000); // TODO: REMOVE IN COMPETITION
    Drive::setBrakeMode(MOTOR_BRAKE_BRAKE);
    GoalGrabber::setNotGrabbing();
    Arm::setArmUp();
    Drive::driveArc(TRACK_WIDTH/2, -0.06, rushSpeed);
    Drive::driveDistanceFeedbackBasic(41, slowSpeed, rushSpeed); // Initial move
    Drive::driveDistance(5, slowSpeed);
    Arm::setArmDown();
    pros::delay(500);
    Drive::driveDistance(-12, rushSpeed); 
    Arm::setArmUp();
    pros::delay(250);
    Drive::turn(-200, turnSpeed);
    Drive::driveDistance(-10, driveSpeed);
    GoalGrabber::setGrabbing();
    Drive::driveDistance(-2, driveSpeed);
    Drive::driveArcDistance(18, 30, rushSpeed);
    Intake::setIntaking();
    Drive::driveArcDistance(18, 8, driveSpeed);
    Conveyor::conveyTime(2000, CONVEYOR_FORWARD_SPEED);
    Intake::setNotMoving();
    pros::delay(200);
    GoalGrabber::setNotGrabbing();
    
}

void rushWithGoalGrab() {
        const int32_t rushSpeed = 100;
        const int32_t driveSpeed = 50;
        const int32_t slowSpeed = 20;
        const int32_t turnSpeed = 50;
        pros::delay(1000); // TODO: REMOVE IN COMPETITION
        Drive::setBrakeMode(MOTOR_BRAKE_BRAKE);
        GoalGrabber::setNotGrabbing();
        Drive::driveArcDistance(72, 35, -rushSpeed);
        Drive::driveArcDistance(72, 7, -driveSpeed); 
        Drive::driveArcDistance(72, 7, -slowSpeed); 
        // Drive::driveArcDistance(TRACK_WIDTH/2, -20, -slowSpeed);
        pros::delay(250);
        GoalGrabber::setGrabbing();

        Drive::driveArcDistance(30, 19, rushSpeed);  // Realign mogo
        GoalGrabber::setNotGrabbing();
        Drive::brake();
        Drive::driveDistance(8, driveSpeed);
        Drive::driveDistance(-10, slowSpeed);
        GoalGrabber::setGrabbing();

        Drive::driveArcDistance(30, 19, rushSpeed); 
        Intake::setIntaking();
        Drive::driveArcDistance(36, 6, driveSpeed);
        Conveyor::conveyTime(2000, CONVEYOR_FORWARD_SPEED);
        Intake::setNotMoving();
        pros::delay(200);
        GoalGrabber::setNotGrabbing();
}
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

void testBasicFeedbackDrive() {
    Drive::setBrakeMode(MOTOR_BRAKE_BRAKE);
    int32_t minPower = 20;
    int32_t maxPower = 40;
    Drive::driveDistanceFeedbackBasic(24, minPower, maxPower);
    pros::delay(2000);
    Drive::driveDistanceFeedbackBasic(-24, minPower, maxPower);
}

void testArc() {
    Drive::setBrakeMode(MOTOR_BRAKE_BRAKE);
    Drive::driveArc(TRACK_WIDTH/2, -0.25, 25); 
}