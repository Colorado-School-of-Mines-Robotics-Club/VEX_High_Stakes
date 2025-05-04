#include "climb.h"
#include "constants.h"

bool Climb::climbValue(false);
enum ClimbState Climb::climbState(ClimbState::NOT_ENGAGED);

pros::adi::DigitalOut Climb::climb(CLIMB_PORT);

void Climb::control(bool climbButton) {
    if(climbButton) {
        if(climbState == ClimbState::NOT_ENGAGED) {
            setEngaged();
        } else if (climbState == ClimbState::ENGAGED) {
            setNotEngaged();
        }
    }
}

bool Climb::isEngaged() {
    return climbValue;
}

void Climb::setClimb(bool engaged) {
    if(engaged) {
        setEngaged();
    } else {
        setNotEngaged();
    }
}

void Climb::setEngaged() {
    climbValue = true;
    climbState = ClimbState::ENGAGED;
    climb.set_value(true);
}

void Climb::setNotEngaged() {
    climbValue = false;
    climbState = ClimbState::NOT_ENGAGED;
    climb.set_value(false);
}



