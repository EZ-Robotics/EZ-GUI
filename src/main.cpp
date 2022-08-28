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

ez::GUI motor_display(
    {{l1, "left 1"},
     {l2, "left 2"},
     {r2, "right 2"},
     {r1, "right 1"},
     {l3, "left 3"},
     {l4, "left 4"},
     {r4, "right 4"},
     {r3, "right 3"},
     {slider_motor, "slider"},
     {intake, "intake"},
     {flywheel, "fly 1"},
     {flywheel2, "fly 2"}});

void initialize() {
  pros::delay(300);
  /*

  // Create new style
  lv_style_t background_style;
  lv_style_copy(&background_style, &lv_style_plain_color);
  background_style.body.main_color = LV_COLOR_RED;
  background_style.body.grad_color = background_style.body.main_color;

  // Create background
  lv_obj_t* temp_background = lv_obj_create(lv_scr_act(), NULL);
  lv_obj_set_size(temp_background, 480, 240);
  lv_obj_set_style(temp_background, &background_style);
  lv_obj_align(temp_background, NULL, LV_ALIGN_CENTER, 0, 0);

  pros::delay(1000);

  // Hide background
  lv_obj_set_hidden(temp_background, true);

  pros::delay(1000);

  // Unhide background
  lv_obj_set_hidden(temp_background, false);

  pros::delay(1000);
  */

  motor_display.enable();
  pros::delay(1000);
  motor_display.disable();
  pros::delay(1000);
  motor_display.enable();
}
void disabled() {}
void competition_initialize() {}
void autonomous() {}

void opcontrol() {
  while (true) {
    pros::delay(50);
  }
}