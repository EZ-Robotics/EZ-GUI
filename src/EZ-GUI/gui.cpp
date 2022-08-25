/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "EZ-GUI/gui.hpp"

#include "pros/screen.h"

using namespace ez;

gui::gui(std::vector<gui_motor_name> motor_name, int color)
    : screenTask([this] { this->screen_task(); }) {
  // Copy parameters over to globals
  for (int i = 0; i < motor_name.size(); i++) {
    motors.push_back(motor_name[i].motor);
    names.push_back(motor_name[i].name);
    temps.push_back(0);
  }
  ACCENT_COLOR = color;

  // Figure out motor box x,y
  calculate_gui();
}

gui::gui(std::vector<gui_int_name> int_name, int color)
    : screenTask([this] { this->screen_task(); }) {
  // Copy parameters over to globals
  for (int i = 0; i < int_name.size(); i++) {
    pros::Motor temp(i);
    motors.push_back(temp);
    names.push_back(int_name[i].name);
    temps.push_back(0);
  }
  ACCENT_COLOR = color;

  // Figure out motor box x,y
  calculate_gui();
}

void gui::calculate_gui() {
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
      boxes.push_back(temp);
      p++;
    }
  }
}

void gui::drawbutton(int color, int xoffset, int yoffset, int width, int height) {
  pros::screen::set_pen(color);
  pros::screen::fill_circle(480 / 2 + width / 2 + xoffset, yoffset, height / 2);
  pros::screen::fill_circle(480 / 2 - width / 2 + xoffset, yoffset, height / 2);
  pros::screen::fill_rect(
      480 / 2 - width / 2 + xoffset,
      height / 2 + yoffset,
      480 / 2 + width / 2 + xoffset,
      -height / 2 + yoffset);
}

void gui::draw_selector_buttons() {
  int width = 40 * 2;
  int boarder = 10;
  int height = 30;
  drawbutton(ACCENT_COLOR, -240, height / 2 + boarder, width, height);
  drawbutton(ACCENT_COLOR, 240, height / 2 + boarder, width, height);
}

void gui::draw_motor_squares() {
  for (int i = 0; i < boxes.size(); i++) {
    double temp = motors[i].get_temperature();
    if (temps[i] != temp) {
      double error = temp - 40.0;
      double percent = 0;
      double opposite_percent = 0;
      if (error > 0) {
        percent = error / (15.0);
        percent = percent > 1.0 ? 1.0 : percent;
      }
      opposite_percent = 1 - percent;

      printf("Motor %s is at %.2fc!\n", names[i].c_str(), temp);

      // double r = (COLOR2R(BACKGROUND_COLOR) * opposite_percent) + (COLOR2R(ACCENT_COLOR) * percent);
      // double g = (fixedCOLOR2G(BACKGROUND_COLOR) * opposite_percent) + (fixedCOLOR2G(ACCENT_COLOR) * percent);
      // double b = (COLOR2B(BACKGROUND_COLOR) * opposite_percent) + (COLOR2B(ACCENT_COLOR) * percent);
      double r = (COLOR2R(ACCENT_COLOR) * opposite_percent) + (COLOR2R(BACKGROUND_COLOR) * percent);
      double g = (fixedCOLOR2G(ACCENT_COLOR) * opposite_percent) + (fixedCOLOR2G(BACKGROUND_COLOR) * percent);
      double b = (COLOR2B(ACCENT_COLOR) * opposite_percent) + (COLOR2B(BACKGROUND_COLOR) * percent);

      pros::screen::set_pen(RGB2COLOR((int)r, (int)g, (int)b));
      pros::screen::fill_rect(boxes[i].x1, boxes[i].y1, boxes[i].x2, boxes[i].y2);

      if (percent != 0) {
        pros::screen::set_pen(ACCENT_COLOR);
        pros::screen::print(pros::E_TEXT_MEDIUM,
                            boxes[i].x1 - 5 + ((boxes[i].x2 - boxes[i].x1) / 2.0),
                            boxes[i].y1 - 5 + ((boxes[i].y2 - boxes[i].y2) / 2.0),
                            "%s", names[i].c_str());
      }
    }

    temps[i] = temp;
  }
}

void gui::screen_task() {
  // 480 x 240
  // Black background
  while (pros::millis() < 250) pros::delay(10);
  pros::screen::set_pen(BACKGROUND_COLOR);
  pros::screen::fill_rect(0, 0, 480, 240);
  pros::delay(100);

  // pros::Controller master(CONTROLLER_MASTER);

  while (true) {
    // Display stuff
    draw_selector_buttons();
    draw_motor_squares();

    // ez::as::auton_selector.print_selected_auton();
    // print_to_screen("Page " + std::to_string(current_auton_page + 1) + "\n" + Autons[current_auton_page].Name);

    /*
    if (!(pros::competition::is_autonomous() && pros::competition::is_disabled())) {
      if (master.get_digital_new_press(DIGITAL_LEFT))
        ez::as::page_down();
      else if (master.get_digital_new_press(DIGITAL_RIGHT))
        ez::as::page_up();
    }
    */

    /*
    pros::screen_touch_status_s_t hi = pros::c::screen_touch_status();
    if (status.y < 40) {
      if (status.x < 50) {
        // left
      } else if (status.x > 480 - 50) {
        // right
      } else {
        // somewhere in the middle
      }
    }
    printf("(%i, %i)\n", status.x, status.y);
    */

    // left
    // 0,0,50,40
    // right
    // 480-50,0,480,40

    pros::delay(50);
  }
}

void gui::disable_gui() {
  // screenTask.suspend();
  temps.clear();
}

void gui::enable_gui() {
  // screenTask.resume();
  temps.clear();
}