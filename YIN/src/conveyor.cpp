#include "conveyor.h"
#include "constants.h"

enum ConveyorState Conveyor::conveyorState(ConveyorState::NOT_MOVING);
pros::Motor Conveyor::conveyorMotor(CONVEYOR_PORT);

Conveyor::Conveyor() {
    conveyorMotor.set_gearing(pros::E_MOTOR_GEAR_BLUE);
}   

void Conveyor::control(bool forwardButton, bool reverseButton) {
    if(conveyorState == ConveyorState::NOT_MOVING) {
        if (forwardButton) {
            setConveyingForward();
        } else if (reverseButton) {
            setConveyingReverse();
        }
    } else if (conveyorState == ConveyorState::FORWARD) {
        if (!forwardButton || reverseButton) {
            setNotMoving();
        }
    } else if (conveyorState == ConveyorState::REVERSE) {
        if (!reverseButton || forwardButton) {
            setNotMoving();
        }
    }
}

void Conveyor::move(int32_t voltage) {
    conveyorMotor.move(voltage);
}

void Conveyor::brake() {
    conveyorMotor.brake();
}

void Conveyor::setConveyingForward() {
    conveyorState = ConveyorState::FORWARD;
    conveyorMotor.move(CONVEYOR_FORWARD_SPEED);
}

void Conveyor::setConveyingReverse() {
    conveyorState = ConveyorState::REVERSE;
    conveyorMotor.move(-CONVEYOR_REVERSE_SPEED);
}

void Conveyor::setNotMoving() {
    conveyorState = ConveyorState::NOT_MOVING;
    conveyorMotor.move(0);
}


