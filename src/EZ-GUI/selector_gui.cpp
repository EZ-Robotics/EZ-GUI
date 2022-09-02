/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "EZ-GUI/gui.hpp"
#include "display/lv_core/lv_obj.h"

using namespace ez;

// Initialize selector buttons
void GUI::selector_buttons_initialize() {
  if (gui_initialized) return;
  int width = 120;
  int height = 40;
  int y = 15;

  lv_obj_t* temp_left;
  temp_left = lv_obj_create(lv_scr_act(), NULL);
  lv_obj_set_size(temp_left, 120, 40);
  lv_obj_set_style(temp_left, &selector_button_style);
  lv_obj_align(temp_left, NULL, LV_ALIGN_IN_TOP_LEFT, 0 - (width / 2), y);

  lv_obj_t* temp_right;
  temp_right = lv_obj_create(lv_scr_act(), NULL);
  lv_obj_set_size(temp_right, 120, 40);
  lv_obj_set_style(temp_right, &selector_button_style);
  lv_obj_align(temp_right, NULL, LV_ALIGN_IN_TOP_LEFT, 240 + (width + (width / 2)), y);

  selector_left = temp_left;
  selector_right = temp_right;
}

// Hide selector buttons
void GUI::selector_buttons_hide(bool hidden) {
  if (!gui_initialized) {
    printf("Selector buttons is uninitialized!  Cannot modify hide state!\n");
    return;
  }

  lv_obj_set_hidden(selector_left, hidden);
  lv_obj_set_hidden(selector_right, hidden);
}

// Initialize selector text
void GUI::selector_text_initialize() {
  if (gui_initialized) return;

  lv_obj_t* temp_selector = lv_label_create(lv_scr_act(), NULL);
  lv_obj_set_style(temp_selector, &selector_text_style);
  lv_label_set_align(temp_selector, LV_LABEL_ALIGN_CENTER);
  lv_label_set_long_mode(temp_selector, LV_LABEL_LONG_ROLL);
  lv_obj_set_width(temp_selector, 350);
  lv_label_set_anim_speed(temp_selector, 125);
  lv_label_set_text(temp_selector, "");
  lv_obj_align(temp_selector, NULL, LV_ALIGN_CENTER, 0, -84);
  selector_text = temp_selector;
}

// Update selector text with parameter
// for public use
void GUI::selector_text_set(std::string text) {
  if (selector_wiggle_text_enabled)
    selector_text_wiggle_set(text);
  else
    selector_text_normal_set(text);
}

// Update without wiggling selector text with parameter
// for private use
void GUI::selector_text_normal_set(std::string text) {
  lv_label_set_text(selector_text, text.c_str());
  lv_obj_align(selector_text, NULL, LV_ALIGN_CENTER, 0, -84);
}

// Update select text with parameter
// ...but it wiggles
// for private use
void GUI::selector_text_wiggle_set(std::string text) {
  int spaces_to_add = 28 - text.length();
  if (spaces_to_add <= 2) {
    selector_text_normal_set(text);
    return;
  }

  std::string spaces;
  for (int i = 0; i < spaces_to_add / 2.0; i++) {
    spaces += " ";
  }
  selector_text_normal_set(spaces + text + spaces);
}

void GUI::selector_text_wiggle_toggle(bool toggle) {
  selector_wiggle_text_enabled = toggle;
}

// Hide selector text
void GUI::selector_text_hide(bool hidden) {
  if (!gui_initialized) {
    printf("Selector text is uninitialized!  Cannot modify hide state!\n");
    return;
  }

  lv_obj_set_hidden(selector_text, hidden);
}