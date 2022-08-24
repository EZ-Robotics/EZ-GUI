/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include "api.h"

#define fixedCOLOR2G(COLOR) ((COLOR >> 8) & 0xff)

namespace ez {
class gui {
 public:
  gui(std::vector<pros::Motor> motor_list, std::vector<std::string> motor_names, int color = 0x00FFC0CB);

  void screen_task();
  void disable_gui();
  void enable_gui();

 private:
  int ACCENT_COLOR = 0x00FFC0CB;
  int BACKGROUND_COLOR = COLOR_BLACK;
  void drawbutton(int color, int xoffset, int yoffset, int width, int height);
  void draw_selector_buttons();
  void draw_motor_squares();
  pros::Task screenTask; 

  std::vector<pros::Motor> motors;
  std::vector<std::string> names;
  std::vector<double> temps;
};
}  // namespace ez