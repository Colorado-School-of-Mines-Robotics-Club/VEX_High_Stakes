#include "goal_grabber.h"
#include "constants.h"

enum GrabState GoalGrabber::grabState (GrabState::NOT_GRABBING);

pros::adi::DigitalOut GoalGrabber::goalGrabber (GOAL_GRABBER_PORT);

void GoalGrabber::control(bool grabButton) {
    if(grabState == GrabState::GRABBING && grabButton) {
        setNotGrabbing();
    } else if (grabState == GrabState::NOT_GRABBING && grabButton) {
        setGrabbing();
    }
}

void GoalGrabber::setGrabbing() {
    grabState = GrabState::GRABBING;
    goalGrabber.set_value(true);
}

void GoalGrabber::setNotGrabbing() {
    grabState = GrabState::NOT_GRABBING;
    goalGrabber.set_value(false);
}

