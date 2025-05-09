#pragma once

#include "main.h"

enum class ConveyorState {
    NOT_MOVING,
    FORWARD,
    REVERSE
};

class Conveyor
{
private:
    static enum ConveyorState conveyorState;
public:
    static pros::Motor conveyorMotor;
    Conveyor();
    static void control(bool forwardButton, bool reverseButton);
    static void move(int32_t speed);
    static void brake();
    static void conveyDistance(double units, int32_t speed);
    static void conveyTime(uint32_t milis, int32_t speed);
    static void setConveyingForward();
    static void setConveyingReverse();
    static void setNotMoving();
    static double getPosition();

    static bool isJammed();

    static void direct(double velocity);
};
