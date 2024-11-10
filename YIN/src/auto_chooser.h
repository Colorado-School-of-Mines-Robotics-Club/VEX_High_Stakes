#pragma once

#include "main.h"
#include <string>

#define AUTOS \
X(DO_NOTHING, "Do nothing :(") \
X(DRIVE_FORWARD, "Just go forward") \
X(DRIVE_FORWARD_CONTACT_LADDER, "Contact Ladder") \
X(DRIVE_FORWARD_RUSH_MOGO, "Simple rush and grab mobile goal") \
X(NUM_AUTOS, "TEST")

#define X(auto, name) auto,
enum class Auto : size_t {
    AUTOS
};
#undef X

#define X(auto, name) name,
char const *auto_name[] = {
    AUTOS
};
#undef X

class AutoChooser
{
    private:
        static bool is_blue;
        static enum Auto selected_auto;
    public:
        static void selectNext();
        static void selectPrev();
        static void toggleColor();
        static std::string getName();
        static bool isBlue();
};