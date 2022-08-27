/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include "api.h"
#include "pros/misc.h"

#define fixedCOLOR2G(COLOR) ((COLOR >> 8) & 0xff)

inline pros::Controller master(pros::E_CONTROLLER_MASTER);

namespace ez {

struct gui_motor_name {
  pros::Motor motor;
  std::string name;
};

struct gui_int_name {
  int motor_port;
  std::string name;
};

class GUI {
 public:
  GUI(std::vector<gui_motor_name> motor_name, lv_color_t accent_color = LV_COLOR_HEX(0xFFC0CB));
  GUI(std::vector<gui_int_name> int_name, lv_color_t accent_color = LV_COLOR_HEX(0xFFC0CB));

  void screen_task();
  void disable_gui();
  void enable_gui();

  // static lv_style_t style_txt;
  lv_style_t box_style;
  lv_style_t slctr_bttn_style;
  lv_style_t bckgnd_style;
  lv_style_t box_txt_style;
  lv_style_t slctr_txt_style;

  bool wiggle_text = false;

 private:
  lv_obj_t* selector_text;
  void set_selector_text(std::string text);
  void set_background_color();
  void initialize_motor_boxes();
  void initialize_selector_buttons();
  void initialize_selector_text();
  void set_wiggling_selector_text(std::string text);

  void set_styles(lv_color_t accent_color, lv_color_t background_color);
  void calculate_motor_boxes();
  lv_color_t ACCENT_COLOR = LV_COLOR_HEX(0xFFC0CB);
  lv_color_t BACKGROUND_COLOR = LV_COLOR_BLACK;
  void update_motor_boxes();
  pros::Task screenTask;

  struct box {
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;
  };
  struct motor_display_constants {
    int rows = 0;
    int columns = 0;
    int box_width = 0;
    int box_height = 0;
    int boarder = 0;
  };
  motor_display_constants display;
  int max_motor_name_length = 0;

  std::vector<pros::Motor> motors;
  std::vector<std::string> names;
  std::vector<double> temps;
  std::vector<box> box_pos;
  std::vector<lv_obj_t> motor_boxes;
  std::vector<lv_obj_t> motor_names;
};
}  // namespace ez