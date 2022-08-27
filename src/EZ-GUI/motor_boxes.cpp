/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "EZ-GUI/gui.hpp"

using namespace ez;

// Calculate how many boxes and dimensions of each box
void GUI::calculate_motor_boxes() {
  int amount_of_motors = motors.size();

  display.boarder = 15;

  // Set constants based on amount of motors
  switch (amount_of_motors) {
    case 1:
      display.rows = 1;
      display.columns = 1;
      break;
    case 2:
      display.rows = 1;
      display.columns = 2;
      break;
    case 3:
      display.rows = 1;
      display.columns = 3;
      break;
    case 4:
      display.rows = 1;
      display.columns = 4;
      break;
    case 5 ... 8:
      display.rows = 2;
      display.columns = 4;
      break;
    case 9 ... 12:
      display.rows = 3;
      display.columns = 4;
      break;
    case 13 ... 16:
      display.boarder = 10;
      display.rows = 4;
      display.columns = 4;
      break;
    case 17 ... 20:
      display.boarder = 10;
      display.rows = 5;
      display.columns = 4;
      break;
    default:
      break;
  }

  // Calculate box dimensions
  display.box_height = (190 - (display.boarder * (display.rows + 1))) / display.rows;
  display.box_width = (480 - (display.boarder * (display.columns + 1))) / display.columns;

  // Calculate each motors (x1, y1) and (x2, y2)
  int p = 0;
  int x_offset = 0;
  for (int j = 1; j <= display.rows; j++) {
    // Check if on the final row, and if the boxes need to be centered
    if (j == display.rows) {
      int boxes_left = amount_of_motors - p;
      if (boxes_left < display.columns) {
        x_offset = (480 - ((display.boarder * (boxes_left + 1)) + (display.box_width * boxes_left))) / 2;
      }
    }
    for (int i = 1; i <= display.columns; i++) {
      if (p == amount_of_motors)
        break;
      box temp;
      temp.x1 = x_offset + (display.boarder * i) + (display.box_width * (i - 1));
      temp.y1 = 240 - ((display.boarder * j) + (display.box_height * (j)));
      temp.x2 = temp.x1 + display.box_width;
      temp.y2 = temp.y1 + display.box_height;
      box_pos.push_back(temp);

      p++;
    }
  }
}

// Initial display + place lgvl objects into vectors for updating later
void GUI::initialize_motor_boxes() {
  for (int i = 0; i < box_pos.size(); i++) {
    // Create motor box
    lv_obj_t* temp_box;
    temp_box = lv_obj_create(lv_scr_act(), NULL);
    lv_obj_set_size(temp_box, display.box_width, display.box_height);
    lv_obj_set_style(temp_box, &box_style);
    lv_obj_align(temp_box, NULL, LV_ALIGN_IN_TOP_LEFT, box_pos[i].x1, box_pos[i].y1);

    // Create text
    lv_obj_t* temp_txt = lv_label_create(lv_scr_act(), NULL);
    lv_obj_set_style(temp_txt, &box_txt_style);
    lv_label_set_align(temp_txt, LV_LABEL_ALIGN_CENTER);
    int x = box_pos[i].x1 + (display.box_width / 2);
    int y = box_pos[i].y1 + (display.box_height / 2);
    lv_obj_set_width(temp_txt, display.box_width);
    lv_label_set_text(temp_txt, names[i].c_str());
    lv_obj_align(temp_txt, NULL, LV_ALIGN_CENTER, x - 240, y - 120);

    // Add box and text objects to vectors
    motor_boxes.push_back(*temp_box);
    motor_names.push_back(*temp_txt);
  }
}

// This is constantly run to update the colors of the boxes as motors heat up
void GUI::update_motor_boxes() {
  for (int i = 0; i < box_pos.size(); i++) {
    // Check if the temperature has updated
    double temp = motors[i].get_temperature();
    if (temps[i] != temp) {
      // Check what the temperature of the motor is
      double error = temp - 40.0;
      double percent = 0;
      double opposite_percent = 0;
      if (error > 0) {
        percent = error / 15.0;
        percent = percent > 1.0 ? 1.0 : percent;
      }
      opposite_percent = 1 - percent;

      printf("Motor %s is at %.2fc!\n", names[i].c_str(), temp);

      // Create new color in-between background_color and accent_color
      lv_color_t new_color;
      // new_color.red = (ACCENT_COLOR.red * opposite_percent) + (BACKGROUND_COLOR.red * percent);
      // new_color.green = (ACCENT_COLOR.green * opposite_percent) + (BACKGROUND_COLOR.green * percent);
      // new_color.blue = (ACCENT_COLOR.blue * opposite_percent) + (BACKGROUND_COLOR.blue * percent);
      new_color.red = (BACKGROUND_COLOR.red * opposite_percent) + (ACCENT_COLOR.red * percent);
      new_color.green = (BACKGROUND_COLOR.green * opposite_percent) + (ACCENT_COLOR.green * percent);
      new_color.blue = (BACKGROUND_COLOR.blue * opposite_percent) + (ACCENT_COLOR.blue * percent);

      // Set new color to box and refresh the object
      box_style.body.main_color = new_color;
      box_style.body.grad_color = box_style.body.main_color;
      lv_obj_refresh_style(&motor_boxes[i]);
    }

    temps[i] = temp;
  }
}