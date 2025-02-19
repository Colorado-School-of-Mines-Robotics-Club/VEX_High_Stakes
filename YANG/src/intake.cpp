#include "intake.h"
#include "constants.h"

enum IntakeState Intake::intakeState(IntakeState::NOT_MOVING);
pros::Motor Intake::intakeMotor(INTAKE_PORT);

Intake::Intake() {
    intakeMotor.set_gearing(pros::E_MOTOR_GEAR_BLUE);
}   

void Intake::control(bool forwardButton, bool reverseButton) {
    if (forwardButton && !reverseButton) {
        setIntaking();
    } else if (reverseButton && !forwardButton) {
        setOuttaking();
    } else {
        setNotMoving();
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
    intakeMotor.move(-OUTTAKE_SPEED);
}

void Intake::setNotMoving() {
    intakeState = IntakeState::NOT_MOVING;
    intakeMotor.move(0);
}

void Intake::direct(double velocity) {
    intakeMotor.move_velocity(velocity);
}



