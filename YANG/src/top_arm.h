#pragma once

#include "main.h"

#define TOP_ARM_DELTA 5
enum class TopArmState {
    APPROACH_MOGO,
    MOGO,
    APPROACH_RING_FROM_MOGO,
    APPROACH_RING_FROM_HIGH_STAKE,
    RING,
    APPROACH_HIGH_STAKE,
    HIGH_STAKE
};

class TopArm{
    private:
        static enum TopArmState topArmState;
        static bool atDesiredPosition();
        static double desiredPosition;
    public:
        TopArm();
        static pros::Motor topArmMotor;
        static void tarePosition();

        static void control(bool mogo_button, bool upper_toggle_button);
        static TopArmState getTopArmState();

        static void approachMogo();
        static void approachRingFromMogo();
        static void approachRingFromHighStake();
        static void approachHighStake();

        static void reachMogo();
        static void reachRing();
        static void reachHighStake();

        static bool atRingPosition();

};