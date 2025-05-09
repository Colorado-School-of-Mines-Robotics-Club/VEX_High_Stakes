#include "conveyor.h"
#include "constants.h"

enum ConveyorState Conveyor::conveyorState(ConveyorState::NOT_MOVING);
pros::Motor Conveyor::conveyorMotor(CONVEYOR_PORT);

Conveyor::Conveyor() {
    conveyorMotor.tare_position();
    conveyorMotor.set_gearing(pros::E_MOTOR_GEAR_BLUE);
}

void Conveyor::control(bool forwardButton, bool reverseButton) {
    if (forwardButton && !reverseButton) {
        setConveyingForward();
    } else if (reverseButton && !forwardButton) {
        setConveyingReverse();
    } else {
        setNotMoving();
    }
}

void Conveyor::move(int32_t speed) {
    conveyorMotor.move(speed);
}

void Conveyor::brake() {
    conveyorMotor.brake();
}

void Conveyor::conveyDistance(double units, int32_t speed) {
    conveyorMotor.tare_position();
    conveyorMotor.move(speed);
    while(conveyorMotor.get_position() < units ) {
        // if(conveyorMotor.get_actual_velocity() < 1) {
        //     conveyorMotor.move(-CONVEYOR_REVERSE_SPEED);
        //     pros::delay(500);
        // }
        // conveyorMotor.move(speed);
        pros::delay(4);
    }
    conveyorMotor.move(0);
}

void Conveyor::conveyTime(uint32_t milis, int32_t power) {
	conveyorMotor.move(power);
	pros::delay(milis);
    conveyorMotor.move(0);
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

double Conveyor::getPosition() {
    return conveyorMotor.get_position();
}

void Conveyor::direct(double velocity) {
    conveyorMotor.move_velocity(velocity);
}

bool Conveyor::isJammed() {
	return false;
    // return conveyorMotor.get_current_draw() > 2000 && conveyorMotor.get_torque() > 0.9 && conveyorMotor.get_actual_velocity() < 1;
}
