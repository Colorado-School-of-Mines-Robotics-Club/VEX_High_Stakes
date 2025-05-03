#pragma once

#include "main.h"


enum class GrabState {
    NOT_GRABBING,
    GRABBING
};

class GoalGrabber
{
private:
    static enum GrabState grabState;
    static pros::adi::DigitalOut goalGrabber;
public:
    static bool grabValue;
    GoalGrabber();
    static void setGrabberValue(bool value);
    /**
     * @brief Driver control to grab the goal
     * 
     * @param grabButton Input digital signal to toggle grab
     */
    static void control(bool grabButton, pros::Controller& controller);
    static void setGrabbing();
    static void setNotGrabbing();
    static void direct(bool grabState);
};
