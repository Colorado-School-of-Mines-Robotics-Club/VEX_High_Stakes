#include "intake.h"
#include "constants.h"

Intake::Intake() :
    IntakeMotor(INTAKE_PORT) {
        IntakeMotor.set_gearing(pros::E_MOTOR_GEAR_BLUE);
    }   

void Intake::intake(int32_t voltage) {
    IntakeMotor.move(voltage);
}

void Intake::brake() {
    IntakeMotor.brake();
}



