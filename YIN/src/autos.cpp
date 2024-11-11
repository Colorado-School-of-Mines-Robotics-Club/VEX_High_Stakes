#include "drive.h"
#include "intake.h"

void doNothing() {

}

void driveForward() {
    pros::lcd::set_text(1, "Driving Forward!");
    Drive::driveDistance(10, 30);
    pros::lcd::clear_line(1);
    pros::lcd::set_text(1, "Finished Driving Forward!");
}

void testIntake() {
    pros::lcd::set_text(1, "Auto!");
    Intake::setIntaking();
    pros::delay(2000);
    Intake::setNotMoving();
    pros::delay(2000);
    Intake::setOuttaking();
    pros::delay(2000);
    Intake::setNotMoving();
}