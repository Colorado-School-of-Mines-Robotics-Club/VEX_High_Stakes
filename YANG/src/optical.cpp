#include "optical.h"
#include "constants.h"

pros::Optical Optical::opticalSensor(OPTICAL_PORT);
enum Color Optical::teamColor(Color::OTHER);
bool Optical::prev_detected_opposite = false;

void Optical::setTeamColor(bool isBlue) {
    if(isBlue) {
        teamColor = Color::BLUE;
    } else {
        teamColor = Color::RED;
    }
}

void Optical::setTeamColor(enum Color color) {
    teamColor = color;
}

bool Optical::oppositeColorDetected() {
    if(teamColor == Color::BLUE) {
        return getColor() == Color::RED;
    } else if (teamColor == Color::RED) {
        return getColor() == Color::BLUE;
    }
    return false;
}

bool Optical::stoppedDetectingOpposite() {
    if(prev_detected_opposite && oppositeColorDetected() == false) {
        prev_detected_opposite = false;
        return true;
    }
    prev_detected_opposite = oppositeColorDetected();
    return false;
}

void Optical::setLED(bool on) {
    if(on) {
        opticalSensor.set_led_pwm(100);
    } else {
        opticalSensor.set_led_pwm(0);
    }
}
pros::Optical Optical::getOptical() {
    return opticalSensor;
}

Color Optical::getColor() {
    double hue = opticalSensor.get_hue();
    if(opticalSensor.get_proximity() >= 250) {
        if(170 < hue && hue < 260 ) {
            return Color::BLUE;
        } else if (340 < hue || hue < 20) {
            return Color::RED;
        }
    }
    return Color::OTHER;
}


std::string Optical::colorString() {
    Color color = Optical::getColor();

    switch(color) {
        case Color::RED:
            return "RED";
        case Color::BLUE:
            return "BLUE";
        // case Color::GREEN:
        //     return "GREEN";
        default:
            return "OTHER";
    }
}

std::string Optical::rgbString() {
    pros::c::optical_rgb_s_t color_values = opticalSensor.get_rgb();
    std::string output = std::format("(%.2f, %.2f, %.2f)", color_values.red, color_values.blue, color_values.green);
    return output;
}

pros::c::optical_rgb_s_t Optical::getRGB() {
    return opticalSensor.get_rgb();
}

double Optical::getHue() {
    return opticalSensor.get_hue();
}

double Optical::getBrightness() {
    return opticalSensor.get_brightness();
}

double Optical::getProximity() {
    return opticalSensor.get_proximity();
}