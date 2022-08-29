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

// Constructor for using motors
GUI::GUI(std::vector<gui_motor_name> motor_name, lv_color_t accent_color)
    : screenTask([this] { this->screen_task(); }) {
  // Copy parameters over to globals
  for (int i = 0; i < motor_name.size(); i++) {
    motors.push_back(motor_name[i].motor);
    names.push_back(motor_name[i].name);
    temps.push_back(0);
  }
  ACCENT_COLOR = accent_color;
  BACKGROUND_COLOR = LV_COLOR_BLACK;
}

// Constructor for using ints
GUI::GUI(std::vector<gui_int_name> int_name, lv_color_t accent_color)
    : screenTask([this] { this->screen_task(); }) {
  // Copy parameters over to globals
  for (int i = 0; i < int_name.size(); i++) {
    pros::Motor temp(i);
    motors.push_back(temp);
    names.push_back(int_name[i].name);
    temps.push_back(0);
  }
  ACCENT_COLOR = accent_color;
  BACKGROUND_COLOR = LV_COLOR_BLACK;
}

void GUI::initialize_background() {
  if (has_initialized) return;

  lv_obj_t* temp_background = lv_obj_create(lv_scr_act(), NULL);
  lv_obj_set_size(temp_background, 480, 240);
  lv_obj_set_style(temp_background, &bckgnd_style);
  lv_obj_align(temp_background, NULL, LV_ALIGN_CENTER, 0, 0);
  background = temp_background;
}

// Hide background
void GUI::hide_background(bool hidden) {
  if (!has_initialized) {
    printf("Background is uninitialized!  Cannot modify hide state!\n");
    return;
  }

  lv_obj_set_hidden(background, hidden);
}

// Set lvgl styles
void GUI::initialize_styles() {
  // Set background style
  lv_style_copy(&bckgnd_style, &lv_style_plain_color);
  bckgnd_style.body.main_color = BACKGROUND_COLOR;
  bckgnd_style.body.grad_color = bckgnd_style.body.main_color;

  // Set box style
  lv_style_copy(&box_style, &lv_style_pretty);
  box_style.body.main_color = BACKGROUND_COLOR;
  box_style.body.grad_color = box_style.body.main_color;

  // Set auto selector button style
  lv_style_copy(&slctr_bttn_style, &lv_style_pretty);
  slctr_bttn_style.body.radius = LV_RADIUS_CIRCLE;
  slctr_bttn_style.body.main_color = ACCENT_COLOR;
  slctr_bttn_style.body.grad_color = slctr_bttn_style.body.main_color;

  // Set box text style
  lv_style_copy(&box_txt_style, &lv_style_plain);
  box_txt_style.text.font = &pros_font_dejavu_mono_20;
  box_txt_style.text.letter_space = 2;
  box_txt_style.text.line_space = 1;
  box_txt_style.text.color = BACKGROUND_COLOR;

  // Set auton selector text style
  lv_style_copy(&slctr_txt_style, &box_txt_style);
  slctr_txt_style.text.color = ACCENT_COLOR;
}

void GUI::screen_task() {
  while (true) {
    while (gui_enabled) {
      // Update motor boxes with motor temperature
      update_motor_boxes();
    }

    pros::delay(50);
  }
}

void GUI::enable() {
  for (int i = 0; i < temps.size(); i++) {
    temps[i] = 0;
  }

  if (!has_initialized) {
    calculate_motor_boxes();  // Figure out motor box x, y
    initialize_styles();      // Style lvgl styles

    initialize_background();
    initialize_motor_boxes();
    initialize_selector_buttons();
    initialize_selector_text();
  } else {
    hide_background(false);
    hide_motor_boxes(false);
    hide_selector_buttons(false);
    hide_selector_text(false);
  }

  gui_enabled = true;
  has_initialized = true;

  // Give cpu time to update display (this function doesnt work without this)
  for (int i = 0; i <= 40; i += 10)
    pros::delay(10);
}

void GUI::disable() {
  hide_background(true);
  hide_motor_boxes(true);
  hide_selector_buttons(true);
  hide_selector_text(true);

  gui_enabled = false;

  // Give cpu time to update display (this function doesnt work without this)
  for (int i = 0; i <= 40; i += 10)
    pros::delay(10);
}