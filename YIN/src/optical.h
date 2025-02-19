#pragma once

#include "main.h"

#define COLOR_THRESHOLD 100

enum class Color {
    BLUE,
    RED,
    GREEN,
    OTHER
};

class Optical
{
private:
    static pros::Optical opticalSensor;
public:
    Optical();
    static void setLED(bool on);
    static pros::Optical getOptical();
    static Color getColor();
    static std::string colorString();
    static std::string rgbString();
    static pros::c::optical_rgb_s_t getRGB();
    static double getHue();
};
