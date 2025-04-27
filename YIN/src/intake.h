#pragma once

#include "main.h"

#define DELAY_TO_STOP 45
#define STOP_TIME 100
#define REVERSE_TIME 200
#define FORWARD_TIME 300


enum class IntakeState {
    NOT_MOVING,
    QUEUE_THROW,
    THROWING,
    INTAKING,
    INTAKING_WITH_CONVEYOR,
    OUTTAKING,
    OUTTAKING_WITH_CONVEYOR,
    UNJAM_REVERSE,
    UNJAM_FORWARD
};

class Intake
{
private:
    static bool colorSortEnabled;
    static int countdown;
    static enum IntakeState intakeState;

public:
    static pros::Motor intakeMotor;
    Intake();
    static void toggleColorSort(bool toggle);

    /**
     * @brief Driver control for intake
     * 
     * @param forwardButton run intake and conveyor forward
     * @param reverseButton run intake and conveyor backwards
     */
    static void control(bool intakeButton, bool bothButton, bool reverseButton, bool oppositeRingDetected);
    static void autoControl(bool intakeOnly, bool both, bool reverse, bool oppositeRingDetected);

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
    static void setQueueThrow();
    static void setThrowing();
    static void setUnjamReverse();
    static void setUnjamForward();

    static void direct(double velocity);
};
