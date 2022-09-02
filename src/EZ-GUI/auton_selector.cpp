/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "EZ-GUI/gui.hpp"

using namespace ez;

void GUI::auton_enable() {
  auton_enabled_is = true;
  selector_text_set("");

  if (auton_enabled_is)
    auton_print();
}

void GUI::auton_disable() {
  auton_enabled_is = false;
  selector_text_set("");
}

// Initialize sd card for auto selector
void GUI::auton_sd_initialize() {
  // If no SD card, return
  if (!IS_SD_CARD) return;

  FILE* as_usd_file_read;
  // If file exists...
  if ((as_usd_file_read = fopen("/usd/auto.txt", "r"))) {
    char l_buf[5];
    fread(l_buf, 1, 5, as_usd_file_read);
    auton_page_current = std::stof(l_buf);
    fclose(as_usd_file_read);
  }
  // If file doesn't exist, create file
  else {
    auton_sd_update();  // Writing to a file that doesn't exist creates the file
    printf("Created auto.txt\n");
  }

  if (auton_page_current > amount_of_autos - 1 || auton_page_current < 0) {
    auton_page_current = 0;
    auton_sd_update();
  }
}

// Update the current page on sd card
void GUI::auton_sd_update() {
  // If no SD card, return
  if (!IS_SD_CARD) return;

  FILE* usd_file_write = fopen("/usd/auto.txt", "w");
  std::string cp_str = std::to_string(auton_page_current);
  char const* cp_c = cp_str.c_str();
  fputs(cp_c, usd_file_write);
  fclose(usd_file_write);
}

// Increase page on auto selector
void GUI::auton_page_up() {
  if (auton_page_current == amount_of_autos - 1)
    auton_page_current = 0;
  else
    auton_page_current++;
  auton_sd_update();
  auton_print();
}

// Decrease page on auto selector
void GUI::auton_page_down() {
  if (auton_page_current == 0)
    auton_page_current = amount_of_autos - 1;
  else
    auton_page_current--;
  auton_sd_update();
  auton_print();
}

// Print selected auton
void GUI::auton_print() {
  if (amount_of_autos == 0) return;

  selector_text_set(autons_and_names[auton_page_current].name);
}

// Call selected auton
void GUI::auton_call() {
  if (amount_of_autos == 0) return;
  autons_and_names[auton_page_current].auton_call();
}

// Get right button
bool GUI::auton_button_right() {
  pros::screen_touch_status_s_t status = pros::c::screen_touch_status();

  if (status.y < 50 && status.x > 480 - 60 && status.touch_status != 0)
    return true;
  return false;
}

// Get new right button
bool GUI::auton_button_right_new() {
  bool cur = auton_button_right();
  if (cur && !auton_button_last_right) {
    auton_button_last_right = true;
    return true;
  }
  auton_button_last_right = cur;
  return false;
}

// Get right button
bool GUI::auton_button_left() {
  pros::screen_touch_status_s_t status = pros::c::screen_touch_status();

  if (status.y < 50 && status.x < 60 && status.touch_status != 0)
    return true;
  return false;
}

// Get new left button
bool GUI::auton_button_left_new() {
  bool cur = auton_button_left();
  if (cur && !auton_button_last_left) {
    auton_button_last_left = true;
    return true;
  }
  auton_button_last_left = cur;
  return false;
}