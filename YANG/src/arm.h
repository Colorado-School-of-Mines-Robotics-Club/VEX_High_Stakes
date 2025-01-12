#pragma once

#include "main.h"

enum class ArmState {
    ARM_UP,
    ARM_DOWN
};

class Arm
{
private:
    static enum ArmState armState;
    static pros::adi::DigitalOut arm;
public:
    static bool armValue;
    Arm();
    /**
     * @brief Controls the arm based on the driver control
     * 
     * @param armButton Input digital signal to toggle arm
     */
    static void control(bool ArmButton);
    /**
     * @brief Moves the arm upwards
     */
    static void setArmUp();
    /**
     * @brief Moves the arm downwaards
     */
    static void setArmDown();
    static void direct(bool armState);
};
