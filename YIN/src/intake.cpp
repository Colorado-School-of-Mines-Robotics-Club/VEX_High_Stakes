#include "intake.h"
#include "conveyor.h"
#include "constants.h"

enum IntakeState Intake::intakeState(IntakeState::NOT_MOVING);
pros::Motor Intake::intakeMotor(INTAKE_PORT);

Intake::Intake() {
    intakeMotor.set_gearing(pros::E_MOTOR_GEAR_BLUE);
}   

void Intake::control(bool forwardButton, bool reverseButton) {
    if(intakeState == IntakeState::NOT_MOVING) {
        if(forwardButton) {
            setIntaking();
            Conveyor::setConveyingForward();
        } else if (reverseButton) {
            setOuttaking();
            Conveyor::setConveyingReverse();
        }
    } else if (intakeState == IntakeState::INTAKING) {
        if(!forwardButton || reverseButton) {
            setNotMoving();
            Conveyor::setNotMoving();
        }
    } else if (intakeState == IntakeState::OUTTAKING) {
        if (!reverseButton || forwardButton) {
            setNotMoving();
            Conveyor::setNotMoving();
        }
    }
}

void Intake::brake() {
    intakeMotor.brake();
}

void Intake::setIntaking() {
    intakeState = IntakeState::INTAKING;
    intakeMotor.move(INTAKE_SPEED);
}

void Intake::setOuttaking() {
    intakeState = IntakeState::OUTTAKING;
    intakeMotor.move(-INTAKE_SPEED);
}

void Intake::setNotMoving() {
    intakeState = IntakeState::NOT_MOVING;
    intakeMotor.move(0);
}


