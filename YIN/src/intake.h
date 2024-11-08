#pragma once

#include "main.h"

enum IntakeState {
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
    static void intake(int32_t voltage);
    static void brake();
    static void setIntaking();
    static void setOuttaking();
    static void setNotMoving();
};
