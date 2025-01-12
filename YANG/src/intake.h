#pragma once

#include "main.h"

enum class IntakeState {
    NOT_MOVING,
    INTAKING,
    OUTTAKING
};

class Intake
{
private:
    static enum IntakeState intakeState;

public:
    static pros::Motor intakeMotor;
    Intake();
    /**
     * @brief Driver control for intake
     * 
     * @param forwardButton run intake and conveyor forward
     * @param reverseButton run intake and conveyor backwards
     */
    static void control(bool forwardButton, bool reverseButton);
        /**
     * @brief Stop intake
     * 
     * 
     */
    static void brake();
    static void setIntaking();
    static void setOuttaking();
    static void setNotMoving();
    static void direct(double velocity);
};
