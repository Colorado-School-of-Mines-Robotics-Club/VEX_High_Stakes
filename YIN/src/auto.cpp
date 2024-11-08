#include "drive.h"
#include "intake.h"

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