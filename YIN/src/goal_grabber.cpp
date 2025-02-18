#include "goal_grabber.h"
#include "constants.h"

bool GoalGrabber::grabValue(false);
enum GrabState GoalGrabber::grabState (GrabState::NOT_GRABBING);

pros::adi::DigitalOut GoalGrabber::goalGrabber (GOAL_GRABBER_PORT);

void GoalGrabber::setGrabberValue(bool value) {
    goalGrabber.set_value(value);
    grabValue = value;
}

void GoalGrabber::control(bool grabButton) {
    if(grabState == GrabState::GRABBING && grabButton) {
        setNotGrabbing();
    } else if (grabState == GrabState::NOT_GRABBING && grabButton) {
        setGrabbing();
    }
}

void GoalGrabber::setGrabbing() {
    grabState = GrabState::GRABBING;
    setGrabberValue(true);
}

void GoalGrabber::setNotGrabbing() {
    grabState = GrabState::NOT_GRABBING;
    setGrabberValue(false);
}

void GoalGrabber::direct(bool grabState) {
    if(grabValue != grabState) {
        setGrabberValue(grabState);
    }
}

