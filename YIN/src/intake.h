#ifndef MINES_INTAKE_H
#define MINES_INTAKE_H

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


#endif // MINES_INTAKE_H
