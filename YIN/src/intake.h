#pragma once

#include "main.h"

class Intake
{
public:
	Intake();
    void intake(int32_t voltage);
    void brake();

private:
    pros::Motor IntakeMotor;
};
