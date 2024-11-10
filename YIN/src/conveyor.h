#pragma once

#include "main.h"

enum class ConveyorState {
    FORWARD,
    REVERSE,
    NOT_MOVING
};

class Conveyor
{
private:
    static enum ConveyorState conveyorState;
    static pros::Motor conveyorMotor;
public:
    Conveyor();
    static void control(bool forwardButton, bool reverseButton);
    static void move(int32_t voltage);
    static void brake();
    static void setConveyingForward();
    static void setConveyingReverse();
    static void setNotMoving();
};
