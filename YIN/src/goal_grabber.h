#pragma once

#include "main.h"

enum GrabState {
    GRABBING,
    NOT_GRABBING
};

class GoalGrabber
{
private:
    static enum GrabState grabState;
    static pros::ADIDigitalOut goalGrabber;
public:
    GoalGrabber();
    static void control(bool button);
    static void setGrabbing();
    static void setNotGrabbing();
};
