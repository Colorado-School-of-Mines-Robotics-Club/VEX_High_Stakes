#include "colored_dropdown.h"

#include <vector>

#include "liblvgl/core/lv_event.h"
#include "liblvgl/core/lv_obj.h"
#include "liblvgl/core/lv_obj_style.h"
#include "liblvgl/extra/layouts/flex/lv_flex.h"
#include "liblvgl/misc/lv_area.h"
#include "liblvgl/misc/lv_color.h"
#include "liblvgl/widgets/lv_dropdown.h"

ColoredDropdown::ColoredDropdown(lv_obj_t* parent) : lvglParent(parent) {
    this->initDropdown();
}

void ColoredDropdown::initDropdown() {
    // Initialize and style the roller
    this->lvglDropdown = lv_dropdown_create(this->lvglParent);

    lv_dropdown_clear_options(this->lvglDropdown);
    lv_dropdown_set_selected_highlight(this->lvglDropdown, true);
    lv_obj_set_width(this->lvglDropdown, lv_pct(100));
    lv_obj_set_flex_grow(this->lvglDropdown, 2);

    lv_obj_add_event_cb(
        this->lvglDropdown,
        [](lv_event_t* event){
            static_cast<ColoredDropdown*>(event->user_data)->valueChangedCallback();
        },
        LV_EVENT_VALUE_CHANGED,
        static_cast<void*>(this)
    );
}

void ColoredDropdown::valueChangedCallback() {
    // Fetch the data for this value
    auto i = lv_dropdown_get_selected(this->lvglDropdown);
    auto elem = this->entries.at(i);

    // Update style of dropdown based on selection
    this->updateStyle();

    // Call the callback
    elem.callback();
}

void ColoredDropdown::updateStyle() {
    // Fetch the data for this value
    auto i = lv_dropdown_get_selected(this->lvglDropdown);
    auto color = this->entries.at(i).color;

    lv_obj_set_style_bg_color(this->lvglDropdown, color, LV_PART_MAIN);

    auto colorInt = color.full;
    if (
        ((colorInt & 0x00FF0000) >> 16)  * 0.299 // R
        + ((colorInt & 0x0000FF00) >> 8) * 0.587 // G
        + (colorInt & 0x000000FF)        * 0.114 // B
        > 145
    ) {
        lv_obj_set_style_text_color(this->lvglDropdown, lv_color_black(), LV_PART_MAIN);
    } else {
        lv_obj_set_style_text_color(this->lvglDropdown, lv_color_white(), LV_PART_MAIN);
    }
}

void ColoredDropdown::addOption(char const* name, lv_color_t color, std::function<void ()> callback, bool isDefault) {
    this->entries.push_back({
        .color = color,
        .callback = callback
    });

    lv_dropdown_add_option(this->lvglDropdown, name, LV_DROPDOWN_POS_LAST);

    if (isDefault) lv_dropdown_set_selected(this->lvglDropdown, this->entries.size() - 1);

    // If this is the first or set as default, we need to recolor based on new selection
    if (this->entries.size() == 1 || isDefault) this->updateStyle();
}

size_t ColoredDropdown::getOptionCount() {
    return this->entries.size();
}
