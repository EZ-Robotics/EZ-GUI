/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "EZ-GUI/gui.hpp"

using namespace ez;

// Initialize sd card for auto selector
void GUI::initialize_selector_sd() {
  // If no SD card, return
  if (!IS_SD_CARD) return;

  FILE* as_usd_file_read;
  // If file exists...
  if ((as_usd_file_read = fopen("/usd/auto.txt", "r"))) {
    char l_buf[5];
    fread(l_buf, 1, 5, as_usd_file_read);
    current_auton_page = std::stof(l_buf);
    fclose(as_usd_file_read);
  }
  // If file doesn't exist, create file
  else {
    selector_update_sd();  // Writing to a file that doesn't exist creates the file
    printf("Created auto.txt\n");
  }

  if (current_auton_page > amount_of_autos - 1 || current_auton_page < 0) {
    current_auton_page = 0;
    selector_update_sd();
  }
}

// Update the current page on sd card
void GUI::selector_update_sd() {
  // If no SD card, return
  if (!IS_SD_CARD) return;

  FILE* usd_file_write = fopen("/usd/auto.txt", "w");
  std::string cp_str = std::to_string(current_auton_page);
  char const* cp_c = cp_str.c_str();
  fputs(cp_c, usd_file_write);
  fclose(usd_file_write);
}

// Increase page on auto selector
void GUI::selector_page_up() {
  if (current_auton_page == amount_of_autos - 1)
    current_auton_page = 0;
  else
    current_auton_page++;
  selector_update_sd();
  print_selected_auton();
}

// Decrease page on auto selector
void GUI::selector_page_down() {
  if (current_auton_page == 0)
    current_auton_page = amount_of_autos - 1;
  else
    current_auton_page--;
  selector_update_sd();
  print_selected_auton();
}

// Print selected auton
void GUI::print_selected_auton() {
  if (amount_of_autos == 0) return;

  set_selector_text(autons_and_names[current_auton_page].name);
}

// Call selected auton
void GUI::call_selected_auton() {
  if (amount_of_autos == 0) return;
  autons_and_names[current_auton_page].auton_call();
}

// Get right button
bool GUI::get_right_button() {
  pros::screen_touch_status_s_t status = pros::c::screen_touch_status();

  if (status.y < 50 && status.x > 480 - 60 && status.touch_status != 0)
    return true;
  return false;
}

// Get new right button
bool GUI::get_new_right_button() {
  bool cur = get_right_button();
  if (cur && !last_right) {
    last_right = true;
    return true;
  }
  last_right = cur;
  return false;
}

// Get right button
bool GUI::get_left_button() {
  pros::screen_touch_status_s_t status = pros::c::screen_touch_status();

  if (status.y < 50 && status.x < 60 && status.touch_status != 0)
    return true;
  return false;
}

// Get new left button
bool GUI::get_new_left_button() {
  bool cur = get_left_button();
  if (cur && !last_left) {
    last_left = true;
    return true;
  }
  last_left = cur;
  return false;
}