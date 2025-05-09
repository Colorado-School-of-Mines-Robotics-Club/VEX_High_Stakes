#include "drive.h"
#include "intake.h"
#include "conveyor.h"
#include "arm.h"
#include "top_arm.h"
#include "goal_grabber.h"
#include "optical.h"
#include "constants.h"

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
        pros::delay(250);
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
        Drive::turn(-90, driveSpeed);
        GoalGrabber::setNotGrabbing();
        Intake::setNotMoving();
        Conveyor::setNotMoving();
        Drive::turn(-90, driveSpeed);

        Drive::driveTime(2000, -driveSpeed, -driveSpeed);

        pros::delay(250);

        Arm::setArmDown();
        Drive::driveTime(4000, slowSpeed, slowSpeed * 0.8);
    }
}

void testAutoIntake(bool isBlue) {
    Drive::setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
    Intake::setNotMovingWithConveyor();
	Optical::setTeamColor(isBlue);
	Optical::setLED(true);
	TopArm::approachMogo();
    while(true) {
        Intake::autoControl(false, true, false, Optical::stoppedDetectingOpposite());
        pros::delay(2);
    }
}

void driveWithSort(double distance, double speed, int max_counts) {
    Drive::setDistance(distance);
    Drive::move(speed, speed);
    // Intake::setIntakingWithConveyor();
    int count = 0;
    while(!Drive::atTarget() && count < max_counts) {
        Intake::autoControl(false, true, false, Optical::stoppedDetectingOpposite());
        count++;
        pros::delay(2);
    }
    // Intake::brake();
    Drive::brake();
}

void dontMoveSort(int counts) {
    Drive::brake();
    // Intake::setIntakingWithConveyor();
    int count = 0;
    while(count < counts) {
        Intake::autoControl(false, true, false, Optical::stoppedDetectingOpposite());
        count++;
        pros::delay(2);
    }
    // Intake::brake();
}

void testCornerSort(bool isBlue) {
    static const int32_t slowSpeed = 30;
    static const int32_t driveSpeed = 50;
    static const int32_t cornerSpeed = 60;
    static const int32_t fastSpeed = 70;
	Drive::setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
    Intake::setNotMovingWithConveyor();
	Optical::setTeamColor(isBlue);
	Optical::setLED(true);
	TopArm::approachMogo();

    GoalGrabber::setGrabbing();
    pros::delay(2000);

    if(isBlue) {
        for(int i = 0; i < 5; i++) {
            driveWithSort(12, driveSpeed, 750);
            dontMoveSort(500);
            driveWithSort(-12, -slowSpeed, 750);
        }
    }
    GoalGrabber::setNotGrabbing();
}

void testDriveWithSort(bool isBlue) {
    static const int32_t slowSpeed = 30;
    static const int32_t driveSpeed = 50;
    static const int32_t cornerSpeed = 60;
    static const int32_t fastSpeed = 70;
    Drive::setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
    Drive::resetHeading();

    Optical::setTeamColor(isBlue);
    Optical::setLED(true);

    GoalGrabber::setGrabbing(); // Grab onto mogo
    pros::delay(2000);
    if(isBlue) {
        driveWithSort(40, slowSpeed, 4000);
        driveWithSort(-40, -slowSpeed, 4000);
        pros::lcd::set_text(4, "Finished!");
    } else {

    }
}

void yinRushNegative(bool isBlue) {
    Drive::resetHeading(false);
    const int32_t rushSpeed = 128;
    const int32_t fastSpeed = 90;
    const int32_t driveSpeed = 50;
    const int32_t slowSpeed = 30;
    const int32_t turnSpeed = 50;
    Optical::setTeamColor(isBlue);
    Optical::enable();
    if (isBlue) {
        GoalGrabber::setNotGrabbing();
        Arm::setArmDown();
        // Drive::driveArc(TRACK_WIDTH/2, 0.052, rushSpeed); // Align with mogo
        Drive::driveDistanceFeedbackBasic(21, rushSpeed, rushSpeed); // Move to mogo
        Drive::driveDistance(7, slowSpeed); // Approach slowly
        Arm::setArmUp();
        Drive::driveDistance(2, slowSpeed); // Approach slowly

        // Drive::driveArc(TRACK_WIDTH/2, -0.052, slowSpeed);

        pros::delay(100);
        Drive::driveDistance(-16, driveSpeed); // Run away
        Arm::setArmDown();
        pros::delay(200);
        // Drive::driveDistance(-6, driveSpeed);
        // Arm::setArmUp();
        // pros::delay(200);
        Drive::driveArc(0, -0.1, turnSpeed); // Start Turn
        Arm::setArmUp(); // Drop goal during turn
        Drive::driveArc(0, -0.3, turnSpeed); // Finish Turn

        Drive::driveDistance(-20, slowSpeed); // Line up Goal
        GoalGrabber::setGrabbing(); // Grab Goal
        driveWithSort(10, driveSpeed, 1000); // Put on Preload
        Drive::brake();
        pros::delay(200);

        Drive::driveArc(0, -0.225, turnSpeed); // Turn towards pile
        driveWithSort(40, driveSpeed, 1000); // Approach corner

        Drive::brake(); // Stop
        // Color sort corner
        for(int i = 0; i < 6; i++) {
            dontMoveSort(250);
            driveWithSort(-8, -slowSpeed, 1000);
            dontMoveSort(250);
            driveWithSort(11, driveSpeed, 1000);
        }

        dontMoveSort(250);
        driveWithSort(-6, -slowSpeed, 750);
        Drive::driveArc(0, 0.47, turnSpeed); // Turn towards stake

        driveWithSort(48, slowSpeed, 5000); // Approach stake

        Intake::setNotMoving();
        Conveyor::setNotMoving();
        Drive::brake();
    } else {
        GoalGrabber::setNotGrabbing();
        Arm::setArmDown();
        // Drive::driveArc(TRACK_WIDTH/2, 0.052, rushSpeed); // Align with mogo
        Drive::driveDistanceFeedbackBasic(22, rushSpeed, rushSpeed); // Move to mogo
        Drive::driveDistance(7, slowSpeed); // Approach slowly
        Arm::setArmUp();
        Drive::driveDistance(2, slowSpeed); // Approach slowly

        // Drive::driveArc(TRACK_WIDTH/2, -0.052, slowSpeed);

        pros::delay(100);
        Drive::driveDistance(-16, driveSpeed); // Run away
        Arm::setArmDown();
        pros::delay(200);
        // Drive::driveDistance(-6, driveSpeed);
        // Arm::setArmUp();
        // pros::delay(200);
        Drive::driveArc(0, -0.1, turnSpeed); // Start Turn
        Arm::setArmUp(); // Drop goal during turn
        Drive::driveArc(0, -0.3, turnSpeed); // Finish Turn

        Drive::driveDistance(-20, slowSpeed); // Line up Goal
        GoalGrabber::setGrabbing(); // Grab Goal
        driveWithSort(10, driveSpeed, 1000); // Put on Preload
        Drive::brake();
        pros::delay(200);

        Drive::driveArc(0, 0.22, turnSpeed); // Turn towards pile
        driveWithSort(40, driveSpeed, 1000); // Approach corner

        Drive::brake(); // Stop
        // Color sort corner
        for(int i = 0; i < 6; i++) {
            dontMoveSort(250);
            driveWithSort(-8, -slowSpeed, 1000);
            dontMoveSort(250);
            driveWithSort(11, driveSpeed, 1000);
        }

        dontMoveSort(250);
        driveWithSort(-6, -slowSpeed, 750);
        Drive::driveArc(0, -0.47, turnSpeed); // Turn towards stake

        driveWithSort(50, slowSpeed, 5000); // Approach stake

        Intake::setNotMoving();
        Conveyor::setNotMoving();
        Drive::brake();
    }
}

// TODO
void yinRushPositive(bool isBlue) {
    Drive::resetHeading(false);
    const int32_t rushSpeed = 128;
    const int32_t fastSpeed = 90;
    const int32_t driveSpeed = 50;
    const int32_t slowSpeed = 30;
    const int32_t turnSpeed = 50;
    Optical::setTeamColor(isBlue);
    Optical::enable();
    if (isBlue) {
    } else {
    }

}

void yinRushCenter(bool isBlue) {
    Drive::resetHeading(false);
    const int32_t rushSpeed = 128;
    const int32_t fastSpeed = 90;
    const int32_t driveSpeed = 50;
    const int32_t slowSpeed = 30;
    const int32_t turnSpeed = 50;
    // pros::Controller controllerMain(pros::E_CONTROLLER_MASTER);

    Optical::setTeamColor(isBlue);
    Optical::enable();
    if (isBlue) {
        GoalGrabber::setNotGrabbing();
        // Drive::driveArc(TRACK_WIDTH/2, 0.052, rushSpeed); // Align with mogo
        Intake::setOuttaking();
        Drive::driveArc(11, -0.185, fastSpeed);
        Arm::setArmDown();

        // Drive::driveDistanceFeedbackBasic(21, rushSpeed, rushSpeed); // Move to mogo
        Drive::driveDistance(18, rushSpeed);
        Drive::driveDistance(6, slowSpeed); // Approach slowly
        Arm::setArmUp();
        Drive::driveDistance(2, slowSpeed); // Approach slowly
        Intake::setNotMoving();

        pros::delay(100);
        Drive::driveDistance(-16, fastSpeed); // Run away
        pros::delay(200);
        // Drive::driveDistance(-6, driveSpeed);
        // Arm::setArmUp();
        // pros::delay(200);
        Drive::driveArc(0, -0.1, turnSpeed);
        Arm::setArmUp(); // Let go of goal mid turn
        Drive::driveArc(0, -0.25, turnSpeed);

        Drive::driveDistance(-5, driveSpeed);
        Drive::driveDistance(-15, slowSpeed);

        GoalGrabber::setGrabbing();
        Drive::driveDistance(6, slowSpeed);
        driveWithSort(8, driveSpeed, 1000);
        Drive::driveArc(0, -0.005, turnSpeed); // Turn towards corner
        driveWithSort(24, driveSpeed, 2000);
        Drive::brake();

        // Color sort corner
        for(int i = 0; i < 8; i++) {
            driveWithSort(8, driveSpeed, 750);
            dontMoveSort(200);
            driveWithSort(-6, -slowSpeed, 750);
            dontMoveSort(200);
        }
        Conveyor::setNotMoving();

        Intake::setNotMovingWithConveyor();
        Drive::driveArc(0, 0.5, turnSpeed); // Turn towards stake

        driveWithSort(48, slowSpeed, 4000);

        Intake::setNotMoving();
        Conveyor::setNotMoving();
        Drive::brake();
    } else {
        GoalGrabber::setNotGrabbing();
        // Drive::driveArc(TRACK_WIDTH/2, 0.052, rushSpeed); // Align with mogo
        Intake::setOuttaking();
        Drive::driveArc(11, 0.145, fastSpeed);
        Arm::setArmDown();

        // Drive::driveDistanceFeedbackBasic(21, rushSpeed, rushSpeed); // Move to mogo
        Drive::driveDistance(23, rushSpeed);
        Drive::driveDistance(7, slowSpeed); // Approach slowly
        Arm::setArmUp();
        Drive::driveDistance(2, slowSpeed); // Approach slowly
        Intake::setNotMoving();

        pros::delay(100);
        Drive::driveDistance(-16, fastSpeed); // Run away
        pros::delay(200);
        // Drive::driveDistance(-6, driveSpeed);
        // Arm::setArmUp();
        // pros::delay(200);
        Drive::driveArc(0, -0.1, turnSpeed);
        Arm::setArmUp(); // Let go of goal mid turn
        Drive::driveArc(0, -0.25, turnSpeed);

        Drive::driveDistance(-5, driveSpeed);
        Drive::driveDistance(-15, slowSpeed);

        GoalGrabber::setGrabbing();
        Drive::driveDistance(6, slowSpeed);
        Drive::driveArc(0, 0.03, turnSpeed); // Turn towards corner

        driveWithSort(8, slowSpeed, 1000);
        driveWithSort(24, driveSpeed, 2000);
        Drive::getIMU()->tare_rotation();
        // Color sort corner
        for(int i = 0; i < 8; i++) {
            driveWithSort(8, driveSpeed, 750);
            dontMoveSort(200);
            driveWithSort(-6, -slowSpeed, 750);
            dontMoveSort(200);
        }
        Conveyor::setNotMoving();
        Intake::setNotMovingWithConveyor();
        // Drive::turnTo(230, turnSpeed); // Turn towards stake
        Drive::driveArc(0, -0.525, turnSpeed); // Turn towards stake

        driveWithSort(34, driveSpeed, 4000);
        driveWithSort(10, slowSpeed, 4000);

        Intake::setNotMoving();
        Conveyor::setNotMoving();
        Drive::brake();
    }

}

// void yinRush(bool isBlue) {
//     Drive::resetHeading();
//     const int32_t rushSpeed = 110;
//     const int32_t fastSpeed = 90;
//     const int32_t driveSpeed = 50;
//     const int32_t slowSpeed = 30;
//     const int32_t turnSpeed = 50;

//     // if (isBlue) {
//         GoalGrabber::setNotGrabbing();
//         Arm::setArmDown();
//         // Drive::driveArc(TRACK_WIDTH/2, 0.052, rushSpeed); // Align with mogo
//         Drive::driveDistanceFeedbackBasic(19, fastSpeed, rushSpeed); // Move to mogo
//         Drive::driveDistance(7, slowSpeed); // Approach slowly
//         Arm::setArmUp();
//         Drive::driveDistance(2, slowSpeed); // Approach slowly

//         // Drive::driveArc(TRACK_WIDTH/2, -0.052, slowSpeed);

//         pros::delay(100);
//         Drive::driveDistance(-12, driveSpeed); // Run away
//         Arm::setArmDown();
//         pros::delay(200);
//         // Drive::driveDistance(-6, driveSpeed);
//         // Arm::setArmUp();
//         // pros::delay(200);
//         Drive::driveArc(0, -0.1, turnSpeed);
//         Arm::setArmUp();
//         Drive::driveArc(0, -0.3, turnSpeed);

//         Drive::driveDistance(-16, slowSpeed);
//         GoalGrabber::setGrabbing();
//         Conveyor::setConveyingForward();
//         Drive::driveDistance(16, slowSpeed);
//         pros::delay(200);

//         Drive::driveArc(0, 0.4, turnSpeed);
//         // Drive::driveDistance(30, slowSpeed);
//         Conveyor::setNotMoving();
//         Drive::brake();


//     // }
// }
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
    pros::Controller controllerMain(pros::E_CONTROLLER_MASTER);

    // Drive::turn(90, turnSpeed); // Turn to face post
    // Drive::turn(-180, turnSpeed); // Turn to face post
    controllerMain.print(2, 0, "start");
    Drive::getIMU()->tare_rotation();
    Drive::turnTo(0, turnSpeed);
    Drive::turnTo(90,turnSpeed);
    controllerMain.print(2, 0, "finish 90");

    Drive::turnTo(0, turnSpeed);
    Drive::turnTo(180, turnSpeed);
    controllerMain.print(2, 0, "finish 180");

    Drive::turnTo(0, turnSpeed);
    Drive::turnTo(270, turnSpeed);
    controllerMain.print(2, 0, "finish 270");

    Drive::turnTo(0, turnSpeed);
    Drive::turnTo(-90, turnSpeed);
    controllerMain.print(2, 0, "finish -90");

    Drive::turnTo(0, turnSpeed);
    Drive::turnTo(-180, turnSpeed);
    controllerMain.print(2, 0, "finish -180");

    Drive::turnTo(0, turnSpeed);
    Drive::turnTo(-270, turnSpeed);
    controllerMain.print(2, 0, "finish all");

    Drive::brake();
}
