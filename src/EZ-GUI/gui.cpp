/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "EZ-GUI/gui.hpp"

#include "display/lv_core/lv_obj.h"
#include "display/lv_core/lv_style.h"
#include "display/lv_fonts/lv_font_builtin.h"
#include "display/lv_objx/lv_label.h"

using namespace ez;

// Constructor for using motors without autons
GUI::GUI(std::vector<gui_motor_name> motor_name, lv_color_t accent_color)
    : screenTask([this] { this->screen_task(); }) {
  // Copy parameters over to globals
  for (int i = 0; i < motor_name.size(); i++) {
    motors.push_back(motor_name[i].motor);
    motor_names.push_back(motor_name[i].name);
    motor_temps.push_back(0);
  }
  ACCENT_COLOR = accent_color;
  BACKGROUND_COLOR = LV_COLOR_BLACK;
  auton_enabled = false;
}

// Constructor for using motors with autons
GUI::GUI(std::vector<gui_motor_name> motor_name, std::vector<auton_and_name> autons, lv_color_t accent_color)
    : screenTask([this] { this->screen_task(); }) {
  // Copy parameters over to globals
  for (int i = 0; i < motor_name.size(); i++) {
    motors.push_back(motor_name[i].motor);
    motor_names.push_back(motor_name[i].name);
    motor_temps.push_back(0);
  }
  ACCENT_COLOR = accent_color;
  BACKGROUND_COLOR = LV_COLOR_BLACK;

  auton_enabled = true;
  amount_of_autos = autons.size();
  auton_page_current = 0;
  autons_and_names = autons;
  auton_sd_initialize();
}

// Constructor for using ints without autos
GUI::GUI(std::vector<gui_int_name> int_name, lv_color_t accent_color)
    : screenTask([this] { this->screen_task(); }) {
  // Copy parameters over to globals
  for (int i = 0; i < int_name.size(); i++) {
    pros::Motor temp(i);
    motors.push_back(temp);
    motor_names.push_back(int_name[i].name);
    motor_temps.push_back(0);
  }
  ACCENT_COLOR = accent_color;
  BACKGROUND_COLOR = LV_COLOR_BLACK;
  auton_enabled = false;
}

// Constructor for using ints with autos
GUI::GUI(std::vector<gui_int_name> int_name, std::vector<auton_and_name> autons, lv_color_t accent_color)
    : screenTask([this] { this->screen_task(); }) {
  // Copy parameters over to globals
  for (int i = 0; i < int_name.size(); i++) {
    pros::Motor temp(i);
    motors.push_back(temp);
    motor_names.push_back(int_name[i].name);
    motor_temps.push_back(0);
  }
  ACCENT_COLOR = accent_color;
  BACKGROUND_COLOR = LV_COLOR_BLACK;

  auton_enabled = true;
  amount_of_autos = autons.size();
  auton_page_current = 0;
  autons_and_names = autons;
  auton_sd_initialize();
}

void GUI::background_initialize() {
  if (gui_initialized) return;

  lv_obj_t* temp_background = lv_obj_create(lv_scr_act(), NULL);
  lv_obj_set_size(temp_background, 480, 240);
  lv_obj_set_style(temp_background, &background_style);
  lv_obj_align(temp_background, NULL, LV_ALIGN_CENTER, 0, 0);
  background = temp_background;
}

// Hide background
void GUI::background_hide(bool hidden) {
  if (!gui_initialized) {
    printf("Background is uninitialized!  Cannot modify hide state!\n");
    return;
  }

  lv_obj_set_hidden(background, hidden);
}

// Set lvgl styles
void GUI::styles_initialize() {
  // Set background style
  lv_style_copy(&background_style, &lv_style_plain_color);
  background_style.body.main_color = BACKGROUND_COLOR;
  background_style.body.grad_color = background_style.body.main_color;

  // Set box style
  lv_style_copy(&box_style, &lv_style_pretty);
  box_style.body.main_color = BACKGROUND_COLOR;
  box_style.body.grad_color = box_style.body.main_color;

  // Set auto selector button style
  lv_style_copy(&selector_button_style, &lv_style_pretty);
  selector_button_style.body.radius = LV_RADIUS_CIRCLE;
  selector_button_style.body.main_color = ACCENT_COLOR;
  selector_button_style.body.grad_color = selector_button_style.body.main_color;

  // Set box text style
  lv_style_copy(&box_txt_style, &lv_style_plain);
  box_txt_style.text.font = &pros_font_dejavu_mono_20;
  box_txt_style.text.letter_space = 2;
  box_txt_style.text.line_space = 1;
  box_txt_style.text.color = BACKGROUND_COLOR;

  // Set auton selector text style
  lv_style_copy(&selector_text_style, &box_txt_style);
  selector_text_style.text.color = ACCENT_COLOR;
}

void GUI::screen_task() {
  while (true) {
    if (gui_enabled) {
      // Update motor boxes with motor temperature
      motor_boxes_update();

      if (auton_enabled) {
        // printf("l(%i, %i)   r(%i, %i)\n", auton_button_left(), auton_button_left_new(), auton_button_right(), auton_button_right_new());
        // Selector with on screen presses
        if (auton_button_right_new()) {
          auton_page_up();
        } else if (auton_button_left_new()) {
          auton_page_down();
        }
      }
    }

    pros::delay(20);
  }
}

void GUI::enable() {
  for (int i = 0; i < motor_temps.size(); i++) {
    motor_temps[i] = 0;
  }

  if (!gui_initialized) {
    motor_boxes_calculate();  // Figure out motor box x, y
    styles_initialize();      // Style lvgl styles

    background_initialize();
    motor_boxes_initialize();
    selector_buttons_initialize();
    selector_text_initialize();
    pong_initialize();
  } else {
    background_hide(false);
    motor_boxes_hide(false);
    selector_buttons_hide(false);
    selector_text_hide(false);
  }

  if (auton_enabled)
    auton_print();

  gui_enabled = true;
  gui_initialized = true;
}

void GUI::disable() {
  gui_enabled = false;

  background_hide(true);
  motor_boxes_hide(true);
  selector_buttons_hide(true);
  selector_text_hide(true);
}