#pragma once

#include <functional>
#include <vector>

#include "liblvgl/core/lv_obj.h"
#include "liblvgl/misc/lv_color.h"

struct colored_dropdown_entry_t {
    lv_color_t color;
    std::function<void ()> callback;
};

class ColoredDropdown {
    private:
        lv_obj_t* lvglParent;
        lv_obj_t* lvglDropdown;
        // lv_style_t lvglStyle;

        std::vector<colored_dropdown_entry_t> entries;

        void initDropdown();

        void valueChangedCallback();
        void updateStyle();
    public:
        ColoredDropdown(lv_obj_t* parent);

        void addOption(char const* name, lv_color_t color, std::function<void ()> callback, bool isDefault);
        size_t getOptionCount();
        bool selectOption(char const* name);
};
