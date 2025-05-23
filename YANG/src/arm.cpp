#include "arm.h"
#include "constants.h"

bool Arm::armValue(false);
enum ArmState Arm::armState (ArmState::ARM_UP);

pros::adi::DigitalOut Arm::arm (ARM_PORT);

void Arm::control(bool armButton) {
    if (armState == ArmState::ARM_UP && armButton) {
        setArmDown();
    } else if (armState == ArmState::ARM_DOWN && armButton) {
        setArmUp();
    }
}

void Arm::setArmUp() {
    armState = ArmState::ARM_UP;
    arm.set_value(false);
    armValue = false;
}

void Arm::setArmDown() {
    armState = ArmState::ARM_DOWN;
    arm.set_value(true);
    armValue = true;
}

void Arm::direct(bool armState) {
    arm.set_value(armState);
}

