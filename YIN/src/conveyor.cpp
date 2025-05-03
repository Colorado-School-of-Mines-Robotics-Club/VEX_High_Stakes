#include "conveyor.h"
#include "constants.h"

enum ConveyorState Conveyor::conveyorState(ConveyorState::NOT_MOVING);
pros::MotorGroup Conveyor::conveyorMotors(CONVEYOR_PORTS);

Conveyor::Conveyor() {
    conveyorMotors.tare_position();
    conveyorMotors.set_gearing(pros::E_MOTOR_GEAR_BLUE);
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

void Conveyor::move(int32_t speed) {
    conveyorMotors.move(speed);
}

void Conveyor::brake() {
    conveyorMotors.brake();
}

void Conveyor::conveyDistance(double units, int32_t speed) {
    conveyorMotors.tare_position();
    conveyorMotors.move(speed);
    while(conveyorMotors.get_position() < units ) {
        // if(conveyorMotors.get_actual_velocity() < 1) {
        //     conveyorMotors.move(-CONVEYOR_REVERSE_SPEED);
        //     pros::delay(500);
        // }
        // conveyorMotors.move(speed);
    }
    conveyorMotors.move(0);
}

void Conveyor::conveyTime(uint32_t milis, int32_t power) {
	conveyorMotors.move(power);
	pros::delay(milis);
    conveyorMotors.move(0);
}

void Conveyor::setConveyingForward() {
    conveyorState = ConveyorState::FORWARD;
    conveyorMotors.move(CONVEYOR_FORWARD_SPEED);
}

void Conveyor::setConveyingReverse() {
    conveyorState = ConveyorState::REVERSE;
    conveyorMotors.move(-CONVEYOR_REVERSE_SPEED);
}

void Conveyor::conveyWithUnjam() {
    if(conveyorState == ConveyorState::NOT_MOVING) {
        setConveyingForward();
    } else if (conveyorState == ConveyorState::FORWARD) {
        if (conveyorMotors.get_current_draw() > 15) {
            setConveyingReverse();
        }
    } else if (conveyorState == ConveyorState::REVERSE) {
        pros::delay(100);
        setConveyingForward();
    }
}

void Conveyor::setNotMoving() {
    conveyorState = ConveyorState::NOT_MOVING;
    conveyorMotors.move(0);
}

double Conveyor::getPosition() {
    return conveyorMotors.get_position();
}

bool Conveyor::isJammed() {
    return conveyorMotors.get_current_draw() > 900 && conveyorMotors.get_torque() > 0.4 && conveyorMotors.get_actual_velocity() < 1;
}


void Conveyor::direct(double velocity) {
    conveyorMotors.move_velocity(velocity);
}


