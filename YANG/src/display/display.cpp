#include "display.h"
#include "colored_dropdown.h"
#include "liblvgl/core/lv_obj.h"
#include "liblvgl/core/lv_obj_pos.h"
#include "liblvgl/core/lv_obj_style.h"
#include "liblvgl/extra/layouts/flex/lv_flex.h"
#include "liblvgl/extra/layouts/grid/lv_grid.h"
#include "liblvgl/lv_api_map.h"
#include "liblvgl/misc/lv_area.h"
#include "liblvgl/misc/lv_color.h"
#include "liblvgl/widgets/lv_img.h"
#include <algorithm>

Display::Display() {
    // Setup the full screen as flexbox
    lv_obj_set_layout(this->screen, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(this->screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align( // Align all items in screen evenly
        this->screen,
        LV_FLEX_ALIGN_START,
        LV_FLEX_ALIGN_START,
        LV_FLEX_ALIGN_START
    );
    lv_obj_set_style_pad_row(this->screen, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_column(this->screen, 0, LV_PART_MAIN);

    // Create a top container for buttons
    this->topContainer = lv_obj_create(this->screen);
    lv_obj_set_flex_grow(this->topContainer, 1);
    lv_obj_set_size(this->topContainer, lv_pct(100), lv_pct(100)); // Leave size to grow
    lv_obj_set_layout(this->topContainer, LV_LAYOUT_FLEX); // Arrange children in flexbox
    lv_obj_set_flex_flow(this->topContainer, LV_FLEX_FLOW_ROW); // Never wrap onto a second row
    lv_obj_set_flex_align( // Align all items in container evenly
        this->topContainer,
        LV_FLEX_ALIGN_SPACE_EVENLY,
        LV_FLEX_ALIGN_SPACE_EVENLY,
        LV_FLEX_ALIGN_SPACE_EVENLY
    );

    // Create a bottom container, for information
    this->bottomContainer = lv_obj_create(this->screen);
    lv_obj_align_to(this->bottomContainer, this->topContainer, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_flex_grow(this->bottomContainer, 2);
    lv_obj_set_size(this->bottomContainer, lv_pct(100), lv_pct(100)); // Leave size to grow
    lv_obj_set_layout(this->bottomContainer, LV_LAYOUT_GRID); // Arrange children in flexbox
    // lv_obj_set_layout(this->bottomContainer, LV_LAYOUT_FLEX); // Arrange children in flexbox
    // lv_obj_set_flex_flow(this->bottomContainer, LV_FLEX_FLOW_ROW_WRAP);
    // lv_obj_set_flex_align( // Align all items in container evenly
    //     this->bottomContainer,
    //     LV_FLEX_ALIGN_SPACE_EVENLY,
    //     LV_FLEX_ALIGN_SPACE_EVENLY,
    //     LV_FLEX_ALIGN_SPACE_EVENLY
    // );
    lv_obj_set_grid_align(this->bottomContainer, LV_GRID_ALIGN_START, LV_GRID_ALIGN_START);
    lv_obj_set_style_bg_color(this->bottomContainer, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_pad_row(this->bottomContainer, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_column(this->bottomContainer, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(this->bottomContainer, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(this->bottomContainer, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(this->bottomContainer, 0, LV_PART_MAIN);
    lv_obj_set_style_outline_width(this->bottomContainer, 0, LV_PART_MAIN);
    lv_obj_clear_flag(this->bottomContainer, LV_OBJ_FLAG_SCROLLABLE);

    // Add image to bottom container, floating
    LV_IMG_DECLARE(img_minesrobotics_logo);
    this->bottomImg = lv_img_create(this->bottomContainer);
    lv_img_set_src(this->bottomImg, &img_minesrobotics_logo);
    lv_img_set_antialias(this->bottomImg, false);
    lv_img_set_size_mode(this->bottomImg, LV_IMG_SIZE_MODE_REAL);
    lv_img_set_zoom(this->bottomImg, 128);
    lv_obj_set_style_img_recolor(this->bottomImg, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_img_recolor_opa(this->bottomImg, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_align(this->bottomImg, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(this->bottomImg, LV_OBJ_FLAG_FLOATING);

    this->autoSelector = new ColoredDropdown(this->topContainer);
    this->colorSelector = new ColoredDropdown(this->topContainer);
}

void Display::addAuto(char const* name, lv_color_t color) {
    this->addAuto(name, color, false);
}

void Display::addAuto(char const* name, lv_color_t color, bool isDefault) {
    auto i = this->autoSelector->getOptionCount();
    this->autoSelector->addOption(name, color, [this, i]{
        *this->autoIndexPtr = i;
    }, isDefault);

    if (isDefault) *autoIndexPtr = i;
}

void Display::setupSelectors(size_t* autoIndexPtr, bool* isBluePtr) {
    this->autoIndexPtr = autoIndexPtr;
    this->isBluePtr = isBluePtr;

    this->colorSelector->addOption("Red", lv_color_make(255, 0, 0), [this]{
        *this->isBluePtr = false;
    }, !*isBluePtr);
    this->colorSelector->addOption("Blue", lv_color_make(0, 0, 255), [this]{
        *this->isBluePtr = true;
    }, *isBluePtr);
}

void Display::setupColors(size_t width, size_t height, lv_color_t initialColors[]) {
    // Remove old objects
    for (size_t i = 0; i < this->colorCount.first * this->colorCount.second; i++) {
        lv_obj_del(this->colorObjects[i]);
    }
    delete this->colorObjects;

    // Setup new objects
    lv_coord_t* col_dsc = new lv_coord_t[width + 1];
    lv_coord_t* row_dsc = new lv_coord_t[height + 1];

    std::fill_n(col_dsc, width, LV_GRID_FR(1));
    std::fill_n(row_dsc, height, LV_GRID_FR(1));

    col_dsc[width] = LV_GRID_TEMPLATE_LAST;
    row_dsc[height] = LV_GRID_TEMPLATE_LAST;

    lv_obj_set_grid_dsc_array(
        this->bottomContainer,
        col_dsc,
        row_dsc
    );

    lv_obj_t** colorObjectsPtr = new lv_obj_t*[width * height];

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            lv_obj_t* block = lv_obj_create(this->bottomContainer);
            colorObjectsPtr[x + y * width] = block;

            lv_obj_set_style_radius(block, 0, LV_PART_MAIN);
            lv_obj_set_style_outline_width(block, 0, LV_PART_MAIN);
            lv_obj_set_style_border_width(block, 0, LV_PART_MAIN);
            lv_obj_set_grid_cell(block, LV_GRID_ALIGN_START, x, 1, LV_GRID_ALIGN_START, y, 1);
            lv_obj_set_size(block, lv_obj_get_width(this->bottomContainer) / width, lv_obj_get_height(this->bottomContainer) / height); // Leave size to grid
            // std::cout << std::endl << initialColors[x + y * height].full << std::endl;
            lv_obj_set_style_bg_color(block, initialColors[x + y * width], LV_PART_MAIN);
        }
    }

    // Save new objects
    this->colorObjects = colorObjectsPtr;
    this->colorCount = std::make_pair(width, height);

    // Bring image to the top again
    lv_obj_move_foreground(this->bottomImg);
};

void Display::setColor(size_t x, size_t y, lv_color_t color) {
    lv_obj_set_style_bg_color(
        this->colorObjects[x + y * this->colorCount.second],
        color,
        LV_PART_MAIN
    );
}
