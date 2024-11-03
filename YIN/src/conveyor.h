#ifndef MINES_CONVEYOR_H
#define MINES_CONVEYOR_H

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


#endif // MINES_CONVEYOR_H
