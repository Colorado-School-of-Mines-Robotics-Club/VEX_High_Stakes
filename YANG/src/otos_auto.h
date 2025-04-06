#pragma once

#include <cstddef>

#define POINT_COUNT 3

const double AUTO_PATH[3][POINT_COUNT] = {
    { 0, 0, 0}, // Start at origin
    { 6, 0, 0}, // Go foward one inch
    { 6, 6, 90}, // Go right one inch
};

void otosAuto();
