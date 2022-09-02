/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "EZ-GUI/gui.hpp"

using namespace ez;

// Calculate how many boxes and dimensions of each box
void GUI::motor_boxes_calculate() {
  int amount_of_motors = motors.size();

  motor_display_constants.boarder = 15;

  // Set constants based on amount of motors
  switch (amount_of_motors) {
    case 1:
      motor_name_length_max = 33;
      motor_display_constants.rows = 1;
      motor_display_constants.columns = 1;
      break;
    case 2:
      motor_name_length_max = 15;
      motor_display_constants.rows = 1;
      motor_display_constants.columns = 2;
      break;
    case 3:
      motor_name_length_max = 10;
      motor_display_constants.rows = 1;
      motor_display_constants.columns = 3;
      break;
    case 4:
      motor_name_length_max = 7;
      motor_display_constants.rows = 1;
      motor_display_constants.columns = 4;
      break;
    case 5 ... 8:
      motor_name_length_max = 7;
      motor_display_constants.rows = 2;
      motor_display_constants.columns = 4;
      break;
    case 9 ... 12:
      motor_name_length_max = 7;
      motor_display_constants.rows = 3;
      motor_display_constants.columns = 4;
      break;
    case 13 ... 16:
      motor_name_length_max = 7;
      motor_display_constants.boarder = 10;
      motor_display_constants.rows = 4;
      motor_display_constants.columns = 4;
      break;
    case 17 ... 20:
      motor_name_length_max = 7;
      motor_display_constants.boarder = 10;
      motor_display_constants.rows = 5;
      motor_display_constants.columns = 4;
      break;
    default:
      break;
  }

  // Calculate box dimensions
  motor_display_constants.box_height = (190 - (motor_display_constants.boarder * (motor_display_constants.rows + 1))) / motor_display_constants.rows;
  motor_display_constants.box_width = (480 - (motor_display_constants.boarder * (motor_display_constants.columns + 1))) / motor_display_constants.columns;

  // Calculate each motors (x1, y1) and (x2, y2)
  int p = 0;
  int x_offset = 0;
  for (int j = 1; j <= motor_display_constants.rows; j++) {
    // Check if on the final row, and if the boxes need to be centered
    if (j == motor_display_constants.rows) {
      int boxes_left = amount_of_motors - p;
      if (boxes_left < motor_display_constants.columns) {
        x_offset = (480 - ((motor_display_constants.boarder * (boxes_left + 1)) + (motor_display_constants.box_width * boxes_left))) / 2;
      }
    }
    for (int i = 1; i <= motor_display_constants.columns; i++) {
      if (p == amount_of_motors)
        break;
      box temp;
      temp.x1 = x_offset + (motor_display_constants.boarder * i) + (motor_display_constants.box_width * (i - 1));
      temp.y1 = 240 - ((motor_display_constants.boarder * j) + (motor_display_constants.box_height * (j)));
      temp.x2 = temp.x1 + motor_display_constants.box_width;
      temp.y2 = temp.y1 + motor_display_constants.box_height;
      motor_box_positions.push_back(temp);

      p++;
    }
  }
}

// Initial display + place lvgl objects into vectors for updating later
void GUI::motor_boxes_initialize() {
  if (gui_initialized) return;
  motor_boxes_hidden = false;

  for (int i = 0; i < motor_box_positions.size(); i++) {
    // Create motor box
    lv_obj_t* temp_box;
    temp_box = lv_obj_create(lv_scr_act(), NULL);
    lv_obj_set_size(temp_box, motor_display_constants.box_width, motor_display_constants.box_height);
    lv_obj_set_style(temp_box, &box_style);
    lv_obj_align(temp_box, NULL, LV_ALIGN_IN_TOP_LEFT, motor_box_positions[i].x1, motor_box_positions[i].y1);

    // Check if text is over allowed length
    if (motor_names[i].length() > motor_name_length_max) {
      printf("Motor %s is too long!  Max of %i characters!\n", motor_names[i].c_str(), motor_name_length_max);
      motor_names[i] = "LENGTH";
    }

    // Create text
    lv_obj_t* temp_txt = lv_label_create(lv_scr_act(), NULL);
    lv_obj_set_style(temp_txt, &box_txt_style);
    lv_label_set_align(temp_txt, LV_LABEL_ALIGN_CENTER);
    int x = motor_box_positions[i].x1 + (motor_display_constants.box_width / 2);
    int y = motor_box_positions[i].y1 + (motor_display_constants.box_height / 2);
    lv_obj_set_width(temp_txt, motor_display_constants.box_width);
    lv_label_set_text(temp_txt, motor_names[i].c_str());
    lv_obj_align(temp_txt, NULL, LV_ALIGN_CENTER, x - 240, y - 120);

    // Add box and text objects to vectors
    motor_boxes_obj.push_back(temp_box);
    motor_names_obj.push_back(temp_txt);
  }
}

// Hide motor boxes`
void GUI::motor_boxes_hide(bool hidden) {
  if (!gui_initialized) {
    printf("Motor names and boxes are uninitialized!  Cannot modify hide state!\n");
    return;
  }

  motor_boxes_hidden = hidden;
  for (int i = 0; i < motor_box_positions.size(); i++) {
    lv_obj_set_hidden(motor_boxes_obj[i], hidden);
    lv_obj_set_hidden(motor_names_obj[i], hidden);
  }
}

// This is constantly run to update the colors of the boxes as motors heat up
void GUI::motor_boxes_update() {
  if (motor_boxes_hidden) return;

  for (int i = 0; i < motor_box_positions.size(); i++) {
    // Check if the temperature has updated
    double temp = motors[i].get_temperature();
    if (motor_temps[i] != temp) {
      // Check what the temperature of the motor is
      double error = temp - 40.0;
      double percent = 0;
      double opposite_percent = 0;
      if (error > 0) {
        percent = error / 15.0;
        percent = percent > 1.0 ? 1.0 : percent;
      }
      opposite_percent = 1 - percent;

      // printf("Motor %s is at %.2fc!\n", names[i].c_str(), temp);

      // Create new color in-between background_color and accent_color
      static lv_color_t new_color;
      // new_color.red = (ACCENT_COLOR.red * opposite_percent) + (BACKGROUND_COLOR.red * percent);
      // new_color.green = (ACCENT_COLOR.green * opposite_percent) + (BACKGROUND_COLOR.green * percent);
      // new_color.blue = (ACCENT_COLOR.blue * opposite_percent) + (BACKGROUND_COLOR.blue * percent);
      new_color.red = (BACKGROUND_COLOR.red * opposite_percent) + (ACCENT_COLOR.red * percent);
      new_color.green = (BACKGROUND_COLOR.green * opposite_percent) + (ACCENT_COLOR.green * percent);
      new_color.blue = (BACKGROUND_COLOR.blue * opposite_percent) + (ACCENT_COLOR.blue * percent);

      // Set new color to box and refresh the object
      box_style.body.main_color = new_color;
      box_style.body.grad_color = box_style.body.main_color;
      lv_obj_refresh_style(motor_boxes_obj[i]);
    }

    motor_temps[i] = temp;
  }
}