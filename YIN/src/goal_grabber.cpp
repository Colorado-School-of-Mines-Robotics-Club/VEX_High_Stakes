#include "goal_grabber.h"
#include "constants.h"

enum GrabState GoalGrabber::grabState(NOT_GRABBING);

pros::ADIDigitalOut goalGrabber (GOAL_GRABBER_PORT);

void GoalGrabber::control(bool button) {
    if(grabState == GRABBING && button) {
        setNotGrabbing();
    } else if (grabState == NOT_GRABBING && button) {
        setGrabbing();
    }
}

void GoalGrabber::setGrabbing() {
    grabState = GRABBING;
    goalGrabber.set_value(true);
}

void GoalGrabber::setNotGrabbing() {
    grabState = NOT_GRABBING;
    goalGrabber.set_value(false);
}

