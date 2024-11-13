#include "drive.h"
#include "intake.h"

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
    if(isBlue) {
        pros::lcd::set_text(1, "Figure Eight!");
        pros::lcd::set_text(2, "Blue!");
        // These delays are to make each action clear for testing
        pros::delay(2000);
        Drive::driveArc(24, 0.5, 3000);
        pros::delay(2000);
        Drive::driveArc(24, -1, 3000);
        pros::delay(2000);
        Drive::driveArc(24, 0.5, 3000);

        pros::delay(2000);
        Drive::driveArc(24, 0.5, -3000);
        pros::delay(2000);
        Drive::driveArc(24, -1, -3000);
        pros::delay(2000);
        Drive::driveArc(24, 0.5, -3000);
        pros::delay(2000);
    } else {
        pros::lcd::set_text(1, "Figure Eight!");
        pros::lcd::set_text(2, "Red!");
        pros::delay(2000);
        Drive::driveArc(24, -0.5, 3000);
        Drive::driveArc(24, 1, 3000);
        Drive::driveArc(24, -0.5, 3000);
        pros::delay(2000);
        Drive::driveArc(24, -0.5, -3000);
        Drive::driveArc(24, 1, -3000);
        Drive::driveArc(24, -0.5, -3000);
        pros::delay(2000);
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