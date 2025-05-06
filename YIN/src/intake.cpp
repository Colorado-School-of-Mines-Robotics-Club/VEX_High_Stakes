#include "intake.h"
#include "conveyor.h"
#include "constants.h"
#include "optical.h"

bool Intake::colorSortEnabled = true;
int Intake::countdown = 0;
enum IntakeState Intake::intakeState(IntakeState::NOT_MOVING);

pros::Motor Intake::intakeMotor(INTAKE_PORT);

Intake::Intake() {
    countdown = 0;
    intakeMotor.set_gearing(pros::E_MOTOR_GEAR_BLUE);
}

void Intake::toggleColorSort(bool toggle, pros::Controller& controller) {
    if(toggle) {
        if(colorSortEnabled) {
            colorSortEnabled = false;
            Optical::disable();
            controller.print(1, 0, "Sort: disabled");
        } else {
            colorSortEnabled = true;
            Optical::enable();
            controller.print(1, 0, "Sort: enabled ");
        }
    }
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
        } else if (oppositeRingDetected && colorSortEnabled) {
            setQueueThrow();
        }
    } else if (intakeState == IntakeState::INTAKING_WITH_CONVEYOR) {
        if ((!bothButton) || reverseButton) {
            setNotMovingWithConveyor();
        } else if (oppositeRingDetected && colorSortEnabled) {
            setQueueThrow();
        }
    } else if (intakeState == IntakeState::OUTTAKING_WITH_CONVEYOR) {
        if (!reverseButton || intakeButton || bothButton) {
            setNotMovingWithConveyor();
        }
    } else if (intakeState == IntakeState::QUEUE_THROW) {
        countdown--;
        if(countdown <= 0 && colorSortEnabled) {
            setThrowing();
        } else if ((!intakeButton && !bothButton) || reverseButton || !colorSortEnabled) {
            countdown = 0;
            setNotMovingWithConveyor();
        }
    } else if(intakeState == IntakeState::THROWING) {
        countdown--;
        if(countdown <= 0 || !colorSortEnabled) {
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


void Intake::autoControl(bool intake, bool both, bool reverse, bool oppositeRingDetected) {
    // pros::lcd::print(3, "%i", Intake::intakeMotor.get_current_draw());
    // pros::lcd::print(4, "%i", Intake::intakeMotor.get_voltage());
    // pros::lcd::print(5, "%f", Intake::intakeMotor.get_torque());
    // pros::lcd::print(6, "%f", Intake::intakeMotor.get_actual_velocity());

    switch(intakeState) {
        case IntakeState::NOT_MOVING:
            if (intake && !both && !reverse) {
                setIntaking();
            } else if (both && !intake && !reverse) {
                setIntakingWithConveyor();
            } else if(reverse && !intake && !both) {
                setOuttakingWithConveyor();
            }
            break;
        case IntakeState::INTAKING:
            if ((!intake && !both) || reverse) {
                setNotMovingWithConveyor();
            } else if (both && !reverse) {
                setIntakingWithConveyor();
            } else if (oppositeRingDetected && colorSortEnabled) {
                setQueueThrow();
            } else if(Conveyor::isJammed()) {
                setUnjamReverse();
            }
        break;
        case IntakeState::INTAKING_WITH_CONVEYOR:
            if ((!both) || reverse) {
                setNotMovingWithConveyor();
            } else if (oppositeRingDetected && colorSortEnabled) {
                setQueueThrow();
            } else if(Conveyor::isJammed()) {
                setUnjamReverse();
        }
        break;
        case IntakeState::OUTTAKING_WITH_CONVEYOR:
            if (!reverse || intake || both) {
                setNotMovingWithConveyor();
            }
        break;
        case IntakeState::QUEUE_THROW:
            countdown--;
            if(countdown <= 0 && colorSortEnabled) {
                setThrowing();
            } else if ((!intake && !both) || reverse || !colorSortEnabled) {
                countdown = 0;
                setNotMovingWithConveyor();
            }
        break;
        case IntakeState::THROWING:
        countdown--;
            if(countdown <= 0 || !colorSortEnabled) {
                countdown = 0;
                if (intake && !both && !reverse) {
                    setIntaking();
                } else if (both && !intake && !reverse) {
                    setIntakingWithConveyor();
                } else if(reverse && !intake && !both) {
                    setOuttakingWithConveyor();
                }
                setNotMovingWithConveyor();
            }
        break;
        case IntakeState::UNJAM_REVERSE:
            countdown--;
            if(countdown <= 0) {
                countdown = 0;
                setUnjamForward();
            }
        break;
        case IntakeState::UNJAM_FORWARD:
            countdown--;
            if(countdown <= 0) {
                countdown = 0;
                if (intake && !both && !reverse) {
                    setIntaking();
                } else if (both && !intake && !reverse) {
                    setIntakingWithConveyor();
                } else if(reverse && !intake && !both) {
                    setOuttakingWithConveyor();
                }
                setNotMovingWithConveyor();
            }
        break;
        default:
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
    countdown = THROW_TIME;
    intakeState = IntakeState::THROWING;
    intakeMotor.move(0);
    Conveyor::move(-CONVEYOR_REVERSE_SPEED);
}

void Intake::setUnjamReverse() {
    countdown = REVERSE_TIME;
    intakeState = IntakeState::UNJAM_REVERSE;
    Conveyor::setConveyingReverse();
    intakeMotor.move(-OUTTAKE_SPEED);
}

void Intake::setUnjamForward() {
    countdown = FORWARD_TIME;
    intakeState = IntakeState::UNJAM_FORWARD;
    Conveyor::setConveyingForward();
    intakeMotor.move(INTAKE_SPEED);
}

bool Intake::isJammed() {
    return intakeMotor.get_current_draw() > 900 && intakeMotor.get_torque() > 0.4 && intakeMotor.get_actual_velocity() < 1;
}

void Intake::direct(double velocity) {
    intakeMotor.move_velocity(velocity);
}



