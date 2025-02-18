#include "arm.h"
#include "constants.h"

bool Arm::armValue(false);
enum ArmState Arm::armState (ArmState::ARM_UP);

pros::adi::DigitalOut Arm::arm0 (ARM_PORT0);
pros::adi::DigitalOut Arm::arm1 (ARM_PORT1);

void Arm::setArmValue(bool value) {
    arm0.set_value(value);
    arm1.set_value(value);

    armValue = value;
}

void Arm::control(bool armButton) {
    if (armState == ArmState::ARM_UP && armButton) {
        setArmDown();
    } else if (armState == ArmState::ARM_DOWN && armButton) {
        setArmUp();
    }
}

void Arm::setArmUp() {
    armState = ArmState::ARM_UP;
    setArmValue(false);
}

void Arm::setArmDown() {
    armState = ArmState::ARM_DOWN;
    setArmValue(true);
}

void Arm::direct(bool armState) {
    if(armValue != armState) {
        setArmValue(armState);
    }
}

