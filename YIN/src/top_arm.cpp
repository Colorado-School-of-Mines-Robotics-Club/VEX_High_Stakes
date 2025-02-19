#include "top_arm.h"
#include "constants.h"

const double MOGO_POSITION = 5;
const double RING_POSITION = -180;
const double HIGH_STAKE_POSITION = -800;

double TopArm::desiredPosition = 0;

enum TopArmState TopArm::topArmState(TopArmState::MOGO);

pros::Motor TopArm::topArmMotor(TOP_ARM_PORT);

TopArm::TopArm() {
    topArmMotor.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
    topArmMotor.set_gearing(pros::E_MOTOR_GEAR_RED);
    topArmMotor.tare_position();
    desiredPosition = 0;
}

bool TopArm::atDesiredPosition() {
    return (topArmMotor.get_position() - TOP_ARM_DELTA <= desiredPosition)
        && (topArmMotor.get_position() + TOP_ARM_DELTA >= desiredPosition);
}

void TopArm::control(bool mogo_button, bool upper_toggle_button) {
    pros::lcd::print(2, "%f", topArmMotor.get_position());
    pros::lcd::print(3, "%f", desiredPosition);
    pros::lcd::print(4, "%i", TopArm::atDesiredPosition());

    switch(topArmState) {
        case TopArmState::APPROACH_MOGO:
            approachMogo();
            pros::lcd::print(1, "APPROACH_MOGO");
            if(atDesiredPosition) {
                reachMogo();
            }
        break;
        case TopArmState::APPROACH_RING_FROM_MOGO:
            approachRingFromMogo();
            pros::lcd::print(1, "APPROACH_RING");
            if(upper_toggle_button) {
                approachHighStake();
            } else if(mogo_button) {
                approachMogo();
            } else if(atDesiredPosition) {
                reachRing();
            }
        break;
        case TopArmState::APPROACH_RING_FROM_HIGH_STAKE:
            approachRingFromHighStake();
            pros::lcd::print(1, "APPROACH_RING");
            if(upper_toggle_button) {
                approachHighStake();
            } else if(mogo_button) {
                approachMogo();
            } else if(atDesiredPosition) {
                reachRing();
            }
        break;
        case TopArmState::APPROACH_HIGH_STAKE:
            approachHighStake();
            pros::lcd::print(1, "APPROACH_HIGH_STAKE");
            if(upper_toggle_button) {
                approachRingFromHighStake();
            } else if(mogo_button) {
                approachMogo();
            } else if(atDesiredPosition) {
                reachHighStake();
            }
        break;
        case TopArmState::MOGO:
            pros::lcd::print(1, "MOGO");
            if(upper_toggle_button) {
                approachRingFromMogo();
            }
        break;
        case TopArmState::RING:
            pros::lcd::print(1, "RING");
            if(upper_toggle_button) {
                approachHighStake();
            } else if(mogo_button) {
                approachMogo();
            }
        break;

        case TopArmState::HIGH_STAKE:
            pros::lcd::print(1, "HIGH_STAKE");
            if(upper_toggle_button) {
               approachRingFromHighStake();
            } else if(mogo_button) {
                approachMogo();
            }
        break;
    }
}

void TopArm::approachMogo() {
    desiredPosition = MOGO_POSITION;
    topArmMotor.move_absolute(MOGO_POSITION, TOP_ARM_SLOW_SPEED);
    topArmState = TopArmState::APPROACH_MOGO;
}

void TopArm::approachRingFromMogo() {
    desiredPosition = RING_POSITION;
    topArmMotor.move_absolute(RING_POSITION, -TOP_ARM_NORMAL_SPEED);
    topArmState = TopArmState::APPROACH_RING_FROM_MOGO;
}

void TopArm::approachRingFromHighStake() {
    desiredPosition = RING_POSITION;
    topArmMotor.move_absolute(RING_POSITION, TOP_ARM_FAST_SPEED);
    topArmState = TopArmState::APPROACH_RING_FROM_HIGH_STAKE;
}

void TopArm::approachHighStake() {
    desiredPosition = HIGH_STAKE_POSITION;
    topArmMotor.move_absolute(HIGH_STAKE_POSITION, -TOP_ARM_FAST_SPEED);
    topArmState = TopArmState::APPROACH_HIGH_STAKE;
}

void TopArm::reachMogo() {
    // topArmMotor.tare_position();
    // topArmMotor.move(0);
    topArmState = TopArmState::MOGO;
}

void TopArm::reachRing() {
    // topArmMotor.move(0);
    topArmState = TopArmState::RING;
}

void TopArm::reachHighStake() {
    // topArmMotor.move(0);
    topArmState = TopArmState::HIGH_STAKE;
}