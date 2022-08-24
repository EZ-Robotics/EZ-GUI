#include "main.h"

// inline std::vector<pros::Motor> all_motors = {l1, l2, r2, r1, l3, l4, r4, r3, slider_motor, intake, flywheel, flywheel2};
// inline std::vector<std::string> motor_names = {"l1", "l2", "r2", "r1", "l3", "l4", "r4", "r3", "sl", "in", "f1", "f2"};
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

ez::gui display(
    {l1, l2, r2, r1, l3, l4, r4, r3, slider_motor, intake, flywheel, flywheel2},
    {"l1", "l2", "r2", "r1", "l3", "l4", "r4", "r3", "sl", "in", "f1", "f2"});

void initialize() {}
void disabled() {}
void competition_initialize() {}
void autonomous() {}

void opcontrol() {
  while (1) {
    pros::delay(20);
  }
}