/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "EZ-GUI/gui.hpp"

using namespace ez;

// Initialize selector buttons
void GUI::initialize_selector_buttons() {
  int width = 120;
  int height = 40;
  int y = 15;

  lv_obj_t* left_button;
  left_button = lv_obj_create(lv_scr_act(), NULL);
  lv_obj_set_size(left_button, 120, 40);
  lv_obj_set_style(left_button, &slctr_bttn_style);
  lv_obj_align(left_button, NULL, LV_ALIGN_IN_TOP_LEFT, 0 - (width / 2), y);

  lv_obj_t* right_button;
  right_button = lv_obj_create(lv_scr_act(), NULL);
  lv_obj_set_size(right_button, 120, 40);
  lv_obj_set_style(right_button, &slctr_bttn_style);
  lv_obj_align(right_button, NULL, LV_ALIGN_IN_TOP_LEFT, 240 + (width + (width / 2)), y);
}

// Initialize selector text
void GUI::initialize_selector_text() {
  lv_obj_t* temp_text = lv_label_create(lv_scr_act(), NULL);
  lv_obj_set_style(temp_text, &slctr_txt_style);
  lv_label_set_align(temp_text, LV_LABEL_ALIGN_CENTER);
  lv_label_set_long_mode(temp_text, LV_LABEL_LONG_ROLL);
  lv_obj_set_width(temp_text, 350);
  lv_label_set_anim_speed(temp_text, 125);
  lv_label_set_text(temp_text, "");
  lv_obj_align(temp_text, NULL, LV_ALIGN_CENTER, 0, -85);
  selector_text = temp_text;
}

// Update selector text with parameter
void GUI::set_selector_text(std::string text) {
  lv_label_set_text(selector_text, text.c_str());
  lv_obj_align(selector_text, NULL, LV_ALIGN_CENTER, 0, -84);
}

// Update select text with parameter
// ...but it wiggles
void GUI::set_wiggling_selector_text(std::string text) {
  int spaces_to_add = 28 - text.length();
  if (spaces_to_add <= 2) {
    set_selector_text(text);
    return;
  }

  std::string spaces;
  for (int i = 0; i < spaces_to_add / 2.0; i++) {
    spaces += "-";
  }
  set_selector_text(spaces + text + spaces);
}