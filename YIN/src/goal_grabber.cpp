#include "goal_grabber.h"
#include "constants.h"

bool GoalGrabber::grabValue(false);
enum GrabState GoalGrabber::grabState (GrabState::NOT_GRABBING);

pros::adi::DigitalOut GoalGrabber::goalGrabber (GOAL_GRABBER_PORT);

void GoalGrabber::setGrabberValue(bool value) {
    goalGrabber.set_value(value);
    grabValue = value;
}

void GoalGrabber::control(bool grabButton, pros::Controller& controller) {
    if(grabState == GrabState::GRABBING && grabButton) {
        controller.print(2, 0, "open   ");
        setNotGrabbing();
    } else if (grabState == GrabState::NOT_GRABBING && grabButton) {
        controller.print(2, 0, "CLAMPED");
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

