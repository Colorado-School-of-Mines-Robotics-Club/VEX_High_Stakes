#include "intake.h"
#include "conveyor.h"
#include "constants.h"

int Intake::countdown = 0;
enum IntakeState Intake::intakeState(IntakeState::NOT_MOVING);

pros::Motor Intake::intakeMotor(INTAKE_PORT);

Intake::Intake() {
    countdown = 0;
    intakeMotor.set_gearing(pros::E_MOTOR_GEAR_BLUE);
}

void Intake::control(bool intakeButton, bool bothButton, bool reverseButton, bool oppositeRingDetected) {
    // pros::lcd::print(7, "Countdown: %i", countdown);

    if(intakeState == IntakeState::NOT_MOVING) {
        if (intakeButton && !bothButton && !reverseButton) {
            setIntaking();
        } else if (bothButton && !intakeButton && !reverseButton) {
            setIntakingWithConveyor();
        } else if(reverseButton && !intakeButton && !bothButton) {
            setOuttakingWithConveyor();
        }
    } else if (intakeState == IntakeState::INTAKING) {
        if ((!intakeButton && !bothButton) || reverseButton) {
            setNotMovingWithConveyor();
        } else if (bothButton && !reverseButton) {
            setIntakingWithConveyor();
        } else if (oppositeRingDetected) {
            setQueueThrow();
        }
    } else if (intakeState == IntakeState::INTAKING_WITH_CONVEYOR) {
        if ((!bothButton) || reverseButton) {
            setNotMovingWithConveyor();
        } else if (oppositeRingDetected) {
            setQueueThrow();
        }
    } else if (intakeState == IntakeState::OUTTAKING_WITH_CONVEYOR) {
        if (!reverseButton || intakeButton || bothButton) {
            setNotMovingWithConveyor();
        }
    } else if (intakeState == IntakeState::QUEUE_THROW) {
        countdown--;
        if(countdown <= 0) {
            setThrowing();
        } else if ((!intakeButton && !bothButton) || reverseButton) {
            countdown = 0;
            setNotMovingWithConveyor();
        }
    } else if(intakeState == IntakeState::THROWING) {
        countdown--;
        if(countdown <= 0) {
            countdown = 0;
            if (intakeButton && !bothButton && !reverseButton) {
                setIntaking();
            } else if (bothButton && !intakeButton && !reverseButton) {
                setIntakingWithConveyor();
            } else if(reverseButton && !intakeButton && !bothButton) {
                setOuttakingWithConveyor();
            }
            setNotMovingWithConveyor();
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

void Intake::setIntakingWithConveyor() {
    intakeState = IntakeState::INTAKING_WITH_CONVEYOR;
    intakeMotor.move(INTAKE_SPEED);
    Conveyor::setConveyingForward();
}

void Intake::setOuttaking() {
    intakeState = IntakeState::OUTTAKING;
    intakeMotor.move(-OUTTAKE_SPEED);
}

void Intake::setOuttakingWithConveyor() {
    intakeState = IntakeState::OUTTAKING_WITH_CONVEYOR;
    intakeMotor.move(-OUTTAKE_SPEED);
    Conveyor::setConveyingReverse();
}

void Intake::setNotMoving() {
    intakeState = IntakeState::NOT_MOVING;
    intakeMotor.move(0);

}
void Intake::setNotMovingWithConveyor() {
    intakeState = IntakeState::NOT_MOVING;
    intakeMotor.move(0);
    Conveyor::setNotMoving();
}

void Intake::setQueueThrow() {
    countdown = DELAY_TO_STOP;
    intakeState = IntakeState::QUEUE_THROW;
}

void Intake::setThrowing() {
    countdown = STOP_TIME;
    intakeState = IntakeState::THROWING;
    intakeMotor.move(0);
    Conveyor::setNotMoving();
}

void Intake::direct(double velocity) {
    intakeMotor.move_velocity(velocity);
}



