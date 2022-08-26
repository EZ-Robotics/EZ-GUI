#include "main.h"

#include "display/lv_core/lv_obj.h"

pros::Motor l1(11, true);
pros::Motor l2(12, true);
pros::Motor l3(13, true);
pros::Motor l4(14, true);
pros::Motor r1(20, false);
pros::Motor r2(19, false);
pros::Motor r3(18, false);
pros::Motor r4(17, false);
pros::Motor flywheel(9, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);  // this makes rpm in degrees
pros::Motor flywheel2(10, false);
pros::Motor intake(16, false);
pros::Motor slider_motor(2, false);

void initialize() {
  pros::delay(500);

  /*
  static lv_style_t style_bkgnd;
  lv_style_copy(&style_bkgnd, &lv_style_plain_color);
  style_bkgnd.body.main_color = LV_COLOR_BLACK;
  style_bkgnd.body.grad_color = style_bkgnd.body.main_color;

  lv_obj_t* obj1;
  obj1 = lv_obj_create(lv_scr_act(), NULL);
  lv_obj_set_size(obj1, 480, 240);
  lv_obj_set_style(obj1, &style_bkgnd);
  lv_obj_align(obj1, NULL, LV_ALIGN_CENTER, 0, 0);

  static lv_style_t style_txt;
  lv_style_copy(&style_txt, &lv_style_plain);
  style_txt.text.font = &lv_font_dejavu_20;
  style_txt.text.letter_space = 2;
  style_txt.text.line_space = 1;
  style_txt.text.color = LV_COLOR_HEX(0xFFC0CB);
  // style_txt.text.color = LV_COLOR_ORANGE;

  lv_obj_t* txt = lv_label_create(lv_scr_act(), NULL);
  lv_obj_set_style(txt, &style_txt);
  lv_obj_set_pos(txt, 100, 100);
  lv_label_set_text(txt, "l1");
  */

  // Setup motors for displaying temperatures to brain
  ez::gui display({
      {l1, "l1"},
      {l2, "l2"},
      {r2, "r2"},
      {r1, "r1"},
      {l3, "l3"},
      {l4, "l4"},
      {r4, "r4"},
      {r3, "r3"},
      {slider_motor, "sl"},
      {intake, "in"},
      {flywheel, "f1"},
      {flywheel2, "f2"},
  });
}
void disabled() {}
void competition_initialize() {}
void autonomous() {}

void opcontrol() {
  while (1) {
    pros::delay(20);
  }
}