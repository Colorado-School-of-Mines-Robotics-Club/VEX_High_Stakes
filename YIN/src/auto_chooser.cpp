#include "auto_chooser.h"

bool AutoChooser::is_blue = true;
enum Auto AutoChooser::selected_auto(Auto::DO_NOTHING);

void AutoChooser::selectNext() {
    selected_auto = static_cast<Auto>(static_cast<int>(selected_auto) + 1 % static_cast<int>(Auto::NUM_AUTOS));
}

void AutoChooser::selectPrev() {
    selected_auto = static_cast<Auto>(static_cast<int>(selected_auto) - 1);
    if (static_cast<int>(selected_auto) < 0) {
        selected_auto = static_cast<Auto>(static_cast<int>(Auto::NUM_AUTOS));
    }
}

void AutoChooser::toggleColor() {
    //TODO implement
}

std::string AutoChooser::getName() {
    return auto_name[static_cast<int>(selected_auto)];
}

bool AutoChooser::isBlue() {
    return is_blue;
}