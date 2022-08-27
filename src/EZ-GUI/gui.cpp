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

  calculate_motor_boxes();                   // Figure out motor box x, y
  set_styles(accent_color, LV_COLOR_BLACK);  // Style lvgl styles
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

  calculate_motor_boxes();                   // Figure out motor box x, y
  set_styles(accent_color, LV_COLOR_BLACK);  // Style lvgl styles
}

// Set the background color
void GUI::set_background_color() {
  if (has_initialized && lv_obj_get_hidden(background)) return;

  lv_style_copy(&bckgnd_style, &lv_style_plain_color);
  bckgnd_style.body.main_color = LV_COLOR_RED;
  bckgnd_style.body.grad_color = bckgnd_style.body.main_color;

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

// Set lgvl styles
void GUI::set_styles(lv_color_t accent_color, lv_color_t background_color) {
  // Set globals for colors
  BACKGROUND_COLOR = background_color;
  ACCENT_COLOR = accent_color;

  // Set box style
  lv_style_copy(&box_style, &lv_style_pretty);
  box_style.body.main_color = background_color;
  box_style.body.grad_color = box_style.body.main_color;

  // Set auto selector button style
  lv_style_copy(&slctr_bttn_style, &lv_style_pretty);
  slctr_bttn_style.body.radius = LV_RADIUS_CIRCLE;
  slctr_bttn_style.body.main_color = accent_color;
  slctr_bttn_style.body.grad_color = slctr_bttn_style.body.main_color;

  // Set box text style
  lv_style_copy(&box_txt_style, &lv_style_plain);
  box_txt_style.text.font = &pros_font_dejavu_mono_20;
  box_txt_style.text.letter_space = 2;
  box_txt_style.text.line_space = 1;
  box_txt_style.text.color = background_color;

  // Set auton selector text style
  lv_style_copy(&slctr_txt_style, &box_txt_style);
  slctr_txt_style.text.color = accent_color;
}

void GUI::screen_task() {
  while (true) {
    while (gui_enabled) {
      // Update motor boxes with motor temperature
      update_motor_boxes();

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
    }

    pros::delay(50);
  }
}

void GUI::enable() {
  printf("Gui enabled!\n");
  for (int i = 0; i < temps.size(); i++) {
    temps[i] = 0;
  }

  if (!has_initialized) {
    set_background_color();
    // initialize_motor_boxes();
    // initialize_selector_buttons();
    // initialize_selector_text();
  }

  else {
    hide_background(false);
    // hide_motor_boxes(false);
    // hide_selector_buttons(false);
    // hide_selector_text(false);
  }

  gui_enabled = true;
  has_initialized = true;
}

void GUI::disable() {
  printf("Gui disabled!\n");
  hide_background(true);
  // hide_motor_boxes(true);
  // hide_selector_buttons(true);
  // hide_selector_text(true);

  gui_enabled = false;
}