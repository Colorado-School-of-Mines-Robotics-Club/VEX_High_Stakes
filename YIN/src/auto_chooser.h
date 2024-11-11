#pragma once

#include "autos.h"

enum class Auto {
    DO_NOTHING,
    DRIVE_FORWARD,
    DRIVE_FORWARD_CONTACT_LADDER,
    DRIVE_FORWARD_RUSH_MOGO,
    NUM_AUTOS
};

class AutoChooser
{
    private:
        static bool is_blue;
        static enum Auto selected_auto;
    public:
        static void runSelected();
        static void selectNext();
        static void selectPrev();
        static void toggleColor();
        static bool isBlue();
        static const char* getName();
};