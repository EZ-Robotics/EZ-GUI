/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "main.h"
#include "pros/misc.hpp"
#include "pros/screen.h"
using namespace ez;

gui::gui(std::vector<pros::Motor> p_motors, std::vector<std::string> p_names, int color)
    : screenTask([this] { this->screen_task(); }) {
  for (int i = 0; i < p_motors.size(); i++) {
    motors.push_back(p_motors[i]);
    names.push_back(p_names[i]);
    temps.push_back(0);
  }
  ACCENT_COLOR = color;
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
  int width = 100;
  int height = 45;
  int boarder = 16;
  int x = 0;
  for (int j = 1; j <= 3; j++) {
    for (int i = 1; i <= 4; i++) {
      double temp = motors[x].get_temperature();
      if (temps[x] != temp) {
        double error = temp - 40.0;
        double percent = 0;
        double opposite_percent = 0;
        if (error > 0) {
          percent = error / (15.0);
          percent = percent > 1.0 ? 1.0 : percent;
          // percent = (double)master.get_analog(ANALOG_LEFT_Y) / 127.0;
        }
        opposite_percent = 1 - percent;

        printf("Motor %s is at %.2fc!\n", names[x].c_str(), temp);

        // double r = (COLOR2R(BACKGROUND_COLOR) * opposite_percent) + (COLOR2R(ACCENT_COLOR) * percent);
        // double g = (fixedCOLOR2G(BACKGROUND_COLOR) * opposite_percent) + (fixedCOLOR2G(ACCENT_COLOR) * percent);
        // double b = (COLOR2B(BACKGROUND_COLOR) * opposite_percent) + (COLOR2B(ACCENT_COLOR) * percent);
        double r = (COLOR2R(ACCENT_COLOR) * opposite_percent) + (COLOR2R(BACKGROUND_COLOR) * percent);
        double g = (fixedCOLOR2G(ACCENT_COLOR) * opposite_percent) + (fixedCOLOR2G(BACKGROUND_COLOR) * percent);
        double b = (COLOR2B(ACCENT_COLOR) * opposite_percent) + (COLOR2B(BACKGROUND_COLOR) * percent);

        int x1 = (boarder * i) + (width * (i - 1));
        int y1 = 240 - ((boarder * j) + (height * (j)));
        int x2 = x1 + width;
        int y2 = y1 + height;

        pros::screen::set_pen(RGB2COLOR((int)r, (int)g, (int)b));
        pros::screen::fill_rect(x1, y1, x2, y2);

        if (percent != 0) {
          pros::screen::set_pen(ACCENT_COLOR);
          pros::screen::print(pros::E_TEXT_MEDIUM, x1 - 5 + ((x2 - x1) / 2.0), y1 - 5 + ((y2 - y1) / 2.0), "%s", names[x].c_str());
        }
      }

      temps[x] = temp;

      x++;
    }
  }
}

void gui::screen_task() {
  // 480 x 240
  // Black background
  while (pros::millis() < 250) pros::delay(10);
  pros::screen::set_pen(BACKGROUND_COLOR);
  pros::screen::fill_rect(0, 0, 480, 240);
  pros::delay(100);

  pros::Controller master(CONTROLLER_MASTER);

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