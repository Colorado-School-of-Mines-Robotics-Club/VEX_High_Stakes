#pragma once

#include "main.h"

class Conveyor
{
private:
    static pros::Motor conveyorMotor;
public:
    Conveyor();
    static void move(int32_t voltage);
    static void brake();
};
