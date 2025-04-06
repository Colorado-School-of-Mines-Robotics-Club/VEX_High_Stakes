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
    static enum Color teamColor;
    static bool prev_detected_opposite;
public:
    Optical();
    static void setTeamColor(bool isBlue);
    static void setTeamColor(enum Color color);
    static bool oppositeColorDetected();
    static bool stoppedDetectingOpposite();
    static void setLED(bool on);
    static pros::Optical getOptical();
    static Color getColor();
    static std::string colorString();
    static std::string rgbString();
    static pros::c::optical_rgb_s_t getRGB();
    static double getHue();
    static double getBrightness();
    static double getProximity();
};
