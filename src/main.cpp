#include "main.h"

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
    {{l1, "left1"},
     {l2, "left2"},
     {r2, "right2"},
     {r1, "right1"},
     {l3, "left3"},
     {l4, "left4"},
     {r4, "right4"},
     {r3, "right3"},
     {slider_motor, "slider"},
     {intake, "intake"},
     {flywheel, "fly 1"},
     {flywheel2, "fly 2"}});

void initialize() {
  pros::delay(500);
}
void disabled() {}
void competition_initialize() {}
void autonomous() {}

void opcontrol() {
  while (true) {
    pros::delay(50);
  }
}