#pragma once

#include "main.h"

enum class ClimbState {
    NOT_ENGAGED,
    ENGAGED
};

class Climb
{
private:
    static enum ClimbState climbState;
    static pros::adi::DigitalOut climb;
public:
    static bool climbValue;
    static void control(bool climbButton);
    static bool isEngaged();
    static void setClimb(bool engaged);
    static void setEngaged();
    static void setNotEngaged();
    static void direct(bool climbState);
};
