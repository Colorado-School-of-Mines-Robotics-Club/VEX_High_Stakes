#include "intake.h"
#include "constants.h"

enum IntakeState Intake::intakeState(NOT_MOVING);
pros::Motor Intake::intakeMotor(INTAKE_PORT);

Intake::Intake() {
    intakeMotor.set_gearing(pros::E_MOTOR_GEAR_BLUE);
}   

void Intake::intake(int32_t voltage) {
    intakeMotor.move(voltage);
}

void Intake::brake() {
    intakeMotor.brake();
}

void Intake::setIntaking() {
    intakeState = INTAKING;
    intakeMotor.move(INTAKE_SPEED);
}

void Intake::setOuttaking() {
    intakeState = OUTTAKING;
    intakeMotor.move(-INTAKE_SPEED);
}

void Intake::setNotMoving() {
    intakeState = NOT_MOVING;
    intakeMotor.move(0);
}


