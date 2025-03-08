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
    static pros::MotorGroup conveyorMotors;
    Conveyor();
    static void control(bool forwardButton, bool reverseButton);
    static void move(int32_t speed);
    static void brake();
    static void conveyDistance(double units, int32_t speed);
    static void conveyTime(uint32_t milis, int32_t speed);
    static void setConveyingForward();
    static void setConveyingReverse();
    static void setNotMoving();
    static void conveyWithUnjam();
    static double getPosition();
    static void direct(double velocity);
};
