#pragma once

#include "colored_dropdown.h"
#include "liblvgl/core/lv_disp.h"
#include <cstddef>

class Display {
    private:
        lv_obj_t* screen = lv_scr_act();
        lv_obj_t* topContainer;
        lv_obj_t* bottomContainer;
        lv_obj_t* bottomImg;

        lv_obj_t** colorObjects = NULL;
        std::pair<size_t, size_t> colorCount = std::make_pair(0, 0);

        ColoredDropdown* autoSelector;
        size_t* autoIndexPtr;

        ColoredDropdown* colorSelector;
        bool* isBluePtr;
    public:
        Display();

        void addAuto(char const* name, lv_color_t color, bool isDefault);
        void addAuto(char const* name, lv_color_t color);

        void setupSelectors(size_t* autoIndexPtr, bool* isBluePtr);
        void setupColors(size_t width, size_t height, lv_color_t colors[]);

        void setColor(size_t x, size_t y, lv_color_t newColor);

        void reloadFromDisk();
};
