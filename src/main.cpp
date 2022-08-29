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

void auton1() {}
void auton2() {}
void auton3() {}

ez::GUI display(
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
     {flywheel2, "fly 2"}},

    {{"Hello 1", auton1},
     {"Hello 2", auton2},
     {"Hello 3", auton3}});

void initialize() {
  pros::delay(300);

  display.enable();
}
void disabled() {}
void competition_initialize() {}

void autonomous() {
  display.call_selected_auton();
}

void opcontrol() {
  while (true) {
    pros::delay(50);
  }
}