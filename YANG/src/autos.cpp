#include "drive.h"
#include "intake.h"
#include "conveyor.h"
#include "arm.h"
#include "goal_grabber.h"
#include "constants.h"

enum MoveSpeed {
    SLOW = 30,
    DRIVE = 50,
    FAST = 70,
    RUSH = 90,
    TURN = 50,
};

void doNothing() {
    pros::lcd::set_text(1, "This does nothing!");
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
    const int32_t fastSpeed = 70;
    const int32_t driveSpeed = 50;
    const int32_t slowSpeed = 25;
    const int32_t turnSpeed = 50;
    Drive::resetHeading();
    // pros::delay(2000); // TODO: REMOVE IN COMPETITION
    Drive::setBrakeMode(MOTOR_BRAKE_BRAKE);

    if(isBlue) {
        GoalGrabber::setNotGrabbing();
        Arm::setArmUp();
        Drive::driveArc(TRACK_WIDTH/2, 0.052, rushSpeed); // Align with mogo
        Drive::driveDistanceFeedbackBasic(44, slowSpeed, rushSpeed); // Move to mogo
        Drive::driveDistance(6, slowSpeed); // Approach slowly
        Drive::driveArc(TRACK_WIDTH/2, -0.052, slowSpeed);
        Arm::setArmDown(); // Grab
        pros::delay(500); 
        Drive::driveDistance(-2, driveSpeed); // Run away
        Drive::driveDistance(-14, rushSpeed); // Run away
        Drive::driveArc(TRACK_WIDTH/2, 0.052, -slowSpeed);
        Arm::setArmUp(); // Leave goal
        pros::delay(350);
        Drive::turn(-130, turnSpeed); // Turn around
        Drive::driveDistance(-10, driveSpeed); // Align goal
        Drive::driveDistance(-8, slowSpeed); 
        GoalGrabber::setGrabbing(); // Pickup goal
        Drive::driveDistance(-2, driveSpeed); // Move a little more
        Drive::driveArcDistance(22, -30, rushSpeed); // Drive to ring
        Intake::setIntaking(); // Pickup ring
        Drive::driveArcDistance(22, -8, driveSpeed); // Keep Driving
        Conveyor::conveyTime(2000, CONVEYOR_FORWARD_SPEED); // Put ring on goal
        Intake::setNotMoving();
        pros::delay(200);

        Drive::turn(180, turnSpeed);
        Arm::setArmDown();
        Drive::driveTime(5000, slowSpeed*0.8, slowSpeed);
    } else {
        Intake::setIntaking();
        Drive::driveDistance(13.25, driveSpeed); // Drive past goal
        pros::delay(250);
        Drive::driveDistance(-3, driveSpeed); // Drive back to post 
        Intake::setNotMoving();
        pros::delay(200);
        Drive::turn(85, turnSpeed); // Turn to face post
        Drive::driveDistance(7, driveSpeed);
        Drive::driveDistance(-13, driveSpeed); // Drive into wall
        Drive::setDriveVelocity(-driveSpeed); // Keep driving into wall
        Intake::setIntaking();
        Conveyor::conveyTime(700, CONVEYOR_FORWARD_SPEED);
        Intake::setNotMoving();
        Drive::setDriveVelocity(20); // Start driving away
        Conveyor::conveyTime(200, CONVEYOR_FORWARD_SPEED);
        Intake::setNotMoving();
        Drive::driveDistance(1.5, driveSpeed);
        Drive::turn(-160, turnSpeed); // Turn to have pickup facing mogo
        Drive::driveDistance(-14, driveSpeed);
        GoalGrabber::setGrabbing(); // Grab onto mogo
        pros::delay(250);
        Conveyor::conveyTime(1200, CONVEYOR_FORWARD_SPEED); // Place ring on mogo
        Drive::driveArc(9, -0.2, driveSpeed);
        Drive::driveDistance(25, driveSpeed);
        Intake::setIntaking();
        Drive::driveDistance(10, driveSpeed);
        Conveyor::conveyTime(1500, CONVEYOR_FORWARD_SPEED);
        Intake::setNotMoving();
        Arm::setArmDown();
        Drive::driveDistance(-12, driveSpeed); // Drive away after picking up rings
        Drive::turn(20, turnSpeed);
        Drive::driveTime(1000, fastSpeed*0.8, fastSpeed);
        Intake::setIntaking();
        Conveyor::setConveyingForward();
        Drive::turn(-120, driveSpeed);
        GoalGrabber::setNotGrabbing();
        Intake::setNotMoving();
        Conveyor::setNotMoving();
        Drive::turn(-60, driveSpeed);  
        
        Drive::driveTime(2000, -driveSpeed, -driveSpeed);
        
        pros::delay(250);

        Arm::setArmDown();
        Drive::driveTime(250, driveSpeed * 0.8, driveSpeed);
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
    static const int32_t rushSpeed = 90;
    static const int32_t fastSpeed = 70;
    static const int32_t driveSpeed = 50;
    static const int32_t slowSpeed = 30;
    static const int32_t turnSpeed = 50;
    pros::delay(2000);
    Drive::setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
    if(isBlue) {
        { // Intake first ring
            Intake::setIntaking();
            Drive::driveDistance(22.5, slowSpeed); // Drive past goal
            pros::delay(250);
            Intake::setNotMoving();
        }
        { // Align to alliance post
            Drive::driveDistance(-3.75, driveSpeed); // Drive back to post 
            pros::delay(200);
            Drive::turn(90, slowSpeed); // Turn to face post
            Drive::driveDistance(8, 20);
            Drive::driveTime(1300, -slowSpeed, -slowSpeed); // Drive into wall
            Drive::driveDistance(0.8, driveSpeed); // Align to Score Ring
        }
        { // Score ring on alliance post
            Intake::setIntaking();
            Conveyor::conveyTime(600, CONVEYOR_FORWARD_SPEED);
            Intake::setNotMoving();
            pros::delay(100);
        }
        { // Turn and grab mogo
            Drive::driveDistance(1.8, driveSpeed);  // Start driving away
            Drive::turn(-185, slowSpeed); // Turn to have pickup facing mogo
            Drive::driveDistance(-5, driveSpeed);
            Drive::driveDistance(-10, slowSpeed);
            GoalGrabber::setGrabbing(); // Grab onto mogo
            pros::delay(250);
        }
        { // Score ring on mogo
            Conveyor::conveyTime(1200, CONVEYOR_FORWARD_SPEED) ; // Place ring on mogo
        }
        { // Intake and score second ring on mogo
            Drive::driveDistance(2.3, driveSpeed);
            Drive::turn(64,turnSpeed);
            Drive::driveDistance(40, driveSpeed); //Drive to Red then Blue stack
            Intake::setIntaking();
            Conveyor::conveyTime(2000, CONVEYOR_FORWARD_SPEED); //Place Red ring on Mogo
        }
        { // Reject blue ring
            Intake::setOuttaking();
            Drive::driveDistance(5, driveSpeed);
            Drive::driveDistance(-5, driveSpeed);
            Intake::setNotMoving();
        }
        { // Sweep corner rings
            Drive::turn(-20, turnSpeed);
            Drive::driveDistance(20, driveSpeed);
            Arm::setArmDown();
            pros::delay(500);
            Drive::driveTime(1000, 55, -55);
            Arm::setArmUp();
        }

        // Place Goal in Corner
        Drive::driveDistance(-4, driveSpeed);
        Drive::driveTime(700, turnSpeed, -turnSpeed);
        Drive::driveTime(2000, -driveSpeed, -driveSpeed);
        GoalGrabber::setNotGrabbing();
        Drive::driveDistance(6, driveSpeed);

        //Leave Alliance Line
        Drive::turn(-45, turnSpeed);
        Drive::driveDistance(10, driveSpeed);
        
    } else {
        { // Intake first ring
            Intake::setIntaking();
            Drive::driveDistance(22.5, slowSpeed); // Drive past goal
            pros::delay(250);
            Intake::setNotMoving();
        }
        { // Align to alliance post
            Drive::driveDistance(-2.75, driveSpeed); // Drive back to post 
            pros::delay(200);
            Drive::turn(-87, slowSpeed); // Turn to face post
            Drive::driveDistance(8, 20);
            Drive::driveTime(1300, -slowSpeed, -slowSpeed); // Drive into wall
            Drive::driveDistance(0.7, driveSpeed); // Align to Score Ring
        }
        { // Score ring on alliance post
            Intake::setIntaking();
            Conveyor::conveyTime(600, CONVEYOR_FORWARD_SPEED);
            Intake::setNotMoving();
            pros::delay(100);
        }
        { // Turn and grab mogo
            Drive::driveDistance(1.5, driveSpeed);  // Start driving away
            Drive::turn(185, slowSpeed); // Turn to have pickup facing mogo
            Drive::driveDistance(-5, driveSpeed);
            Drive::driveDistance(-10, slowSpeed);
            GoalGrabber::setGrabbing(); // Grab onto mogo
            pros::delay(250);
        }
        { // Score ring on mogo
            Conveyor::conveyTime(1200, CONVEYOR_FORWARD_SPEED) ; // Place ring on mogo
        }
        { // Intake and score second ring on mogo
            Drive::driveDistance(2, driveSpeed);
            Drive::turn(-67,turnSpeed);
            Drive::driveDistance(40, driveSpeed); //Drive to Red then Blue stack
            Intake::setIntaking();
            Conveyor::conveyTime(2000, CONVEYOR_FORWARD_SPEED); //Place Red ring on Mogo
        }
        { // Reject blue ring
            Intake::setOuttaking();
            Drive::driveDistance(5, driveSpeed);
            Drive::driveDistance(-5, driveSpeed);
            Intake::setNotMoving();
        }
        { // Sweep corner rings
            Drive::turn(20, turnSpeed);
            Drive::driveDistance(20, driveSpeed);
            Arm::setArmDown();
            pros::delay(500);
            Drive::driveTime(1000, 55, -55);
            Arm::setArmUp();
        }

        // Place Goal in Corner
        Drive::driveDistance(-4, driveSpeed);
        Drive::driveTime(700, -turnSpeed, turnSpeed);
        Drive::driveTime(2000, -driveSpeed, -driveSpeed);
        GoalGrabber::setNotGrabbing();
        Drive::driveDistance(6, driveSpeed);

        //Leave Alliance Line
        Drive::turn(45, turnSpeed);
        Drive::driveDistance(10, driveSpeed);
    }

}

// Test autos

void driveForward(double distance) {
    Drive::setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
    pros::lcd::set_text(1, "Driving Forward!");
    Drive::driveDistance(distance, 50);
    pros::lcd::clear_line(1);
    pros::lcd::set_text(1, "Finished Driving Forward!");
}

void rotate(double distance) {
    Drive::resetHeading();
    Drive::setBrakeMode(MOTOR_BRAKE_BRAKE);
    pros::lcd::set_text(1, "Rotating");
    Drive::turn(720, 50);
    pros::lcd::clear_line(1);
    pros::lcd::set_text(1, "Finished Rotating!");
}

void driveCircle(double radius) {
    Drive::resetHeading();
    Drive::setBrakeMode(MOTOR_BRAKE_BRAKE);
    pros::delay(2);
    pros::lcd::set_text(1, "Circle");
    Drive::driveArc(radius, 2, 50);
    Drive::setDriveVelocity(0);
    pros::lcd::clear_line(1);
    pros::lcd::set_text(1, "Finished Circle!");
}

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

void driveDistanceGyro(double distance) {
    Drive::resetHeading();
    pros::delay(2000);
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

void rotateTest() {
    int turnSpeed = 50;
    Drive::resetHeading();
    Drive::setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
    Drive::turn(90, turnSpeed); // Turn to face post
    Drive::turn(-180, turnSpeed); // Turn to face post
}   