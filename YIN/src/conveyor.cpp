#include "conveyor.h"
#include "constants.h"

Conveyor::Conveyor() :
    conveyorMotor(CONVEYOR_PORT) {
        conveyorMotor.set_gearing(pros::E_MOTOR_GEAR_BLUE);
    }   

void Conveyor::move(int32_t voltage) {
    conveyorMotor.move(voltage);
}

void Conveyor::brake() {
    conveyorMotor.brake();
}



