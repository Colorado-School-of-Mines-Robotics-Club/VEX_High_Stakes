#pragma once

#include "main.h"

enum class IntakeState {
    INTAKING,
    OUTTAKING,
    NOT_MOVING
};

class Intake
{
private:
    static enum IntakeState intakeState;
    static pros::Motor intakeMotor;
public:
    Intake();
    static void control(bool forwardButton, bool reverseButton);
    static void brake();
    static void setIntaking();
    static void setOuttaking();
    static void setNotMoving();
};
