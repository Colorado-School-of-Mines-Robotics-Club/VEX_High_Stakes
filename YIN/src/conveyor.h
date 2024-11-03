#pragma once

#include "main.h"

class Conveyor
{
public:
	Conveyor();
    void move(int32_t voltage);
    void brake();

private:
    pros::Motor conveyorMotor;
};
