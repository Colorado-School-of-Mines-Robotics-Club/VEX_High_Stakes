#pragma once

#include "main.h"

#define DELAY_TO_STOP 40
#define STOP_TIME 200

enum class IntakeState {
    NOT_MOVING,
    THROWING,
    INTAKING,
    INTAKING_WITH_CONVEYOR,
    OUTTAKING,
    OUTTAKING_WITH_CONVEYOR,
};

class Intake
{
private:
    static int countdown;
    static enum IntakeState intakeState;

public:
    static pros::Motor intakeMotor;
    Intake();
    /**
     * @brief Driver control for intake
     * 
     * @param forwardButton run intake and conveyor forward
     * @param reverseButton run intake and conveyor backwards
     */
    static void control(bool intakeButton, bool bothButton, bool reverseButton, bool oppositeRingDetected);


        /**
     * @brief Stop intake
     * 
     * 
     */
    static void brake();
    static void setIntaking();
    static void setIntakingWithConveyor();
    static void setOuttaking();
    static void setOuttakingWithConveyor();
    static void setNotMoving();
    static void setNotMovingWithConveyor();
    static void direct(double velocity);
};
