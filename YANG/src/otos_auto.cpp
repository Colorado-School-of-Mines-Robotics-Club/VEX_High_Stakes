#include "otos_auto.h"
#include "otos.h"
#include "drive.h"
#include "intake.h"
#include "conveyor.h"
#include "arm.h"
#include "goal_grabber.h"
#include "constants.h"
#include <cstring>

// TODO: Curves?
void otosAuto() {
    // Setup everything
    OtosSensor::setup();
    Drive::resetHeading();

    // Follow path
    for (size_t i = 0; i < POINT_COUNT; i++) {
        // Calculate direction necessary, and turn that way
        // Drive towards it until we get there, with speed/time estimates along the way
    }
}
