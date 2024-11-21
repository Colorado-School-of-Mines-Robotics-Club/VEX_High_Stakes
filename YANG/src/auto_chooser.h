#pragma once

#include "autos.h"

const int NUM_AUTOS = static_cast<int>(static_cast<int>(Auto::NUM_AUTOS));

enum class Auto {
    DO_NOTHING,
    TEST,
    DRIVE_FORWARD,
    DRIVE_FORWARD_CONTACT_LADDER,
    DRIVE_FORWARD_RUSH_MOGO,
    FIGURE_EIGHT,
    NUM_AUTOS
};

class AutoChooser
{
    private:
        static bool is_blue;
        static enum Auto selected_auto;
    public:
        static void runSelected();
        static void selectNext(int amount = 1);
        static void selectPrev();
        static void toggleColor();
        static bool isBlue();
        static const char* getName();
};