#include "auto_chooser.h"

bool AutoChooser::is_blue = true;
enum Auto AutoChooser::selected_auto(Auto::DO_NOTHING);

void AutoChooser::runSelected() {
    switch(selected_auto) {
        case Auto::DO_NOTHING:
            doNothing();
        break;
        case Auto::TEST:
            rotateOnce();
        break;
        case Auto::DRIVE_FORWARD:
            driveForward();
        break;
        case Auto::DRIVE_FORWARD_CONTACT_LADDER:
            driveForwardContactLadder();
        break;
        case Auto::DRIVE_FORWARD_RUSH_MOGO:
            driveForwardRushMogo();
        break;
        case Auto::FIGURE_EIGHT:
            figureEight(is_blue);
        break;
        default:
            doNothing();
        break;
    }
}

void AutoChooser::selectNext() {
    selected_auto = static_cast<Auto>((static_cast<int>(selected_auto) + 1) % static_cast<int>(Auto::NUM_AUTOS));
}

void AutoChooser::selectPrev() {
    if (static_cast<int>(selected_auto) - 1 < 0) {
        selected_auto = static_cast<Auto>(static_cast<int>(Auto::NUM_AUTOS) - 1);
    } else {
        selected_auto = static_cast<Auto>(static_cast<int>(selected_auto) - 1);
    }
}

void AutoChooser::toggleColor() {
    is_blue = !is_blue;
}

bool AutoChooser::isBlue() {
    return is_blue;
}

const char* AutoChooser::getName() {
    switch(selected_auto) {
        case Auto::DO_NOTHING:
            return "Do nothing :(";
        break;
        case Auto::TEST:
            return "Test program";
        break;
        case Auto::DRIVE_FORWARD:
            return "Just go forward";
        break;
        case Auto::DRIVE_FORWARD_CONTACT_LADDER:
            return "Just contact ladder";
        break;
        case Auto::DRIVE_FORWARD_RUSH_MOGO:
            return "Simple rush and grab mogo";
        break;
        case Auto::FIGURE_EIGHT:
            return "Figure eight!";
        default:
            return "This shouldn't happen";
        break;
    }
}