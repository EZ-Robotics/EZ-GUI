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

void initialize() {
  pros::delay(500);

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