#include "main.h"

#include "pros/misc.hpp"

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

    {{"Auto 1", auton1},
     {"Auto 2", auton2},
     {"Auto 3", auton3}});

pros::ADIDigitalIn increase('F');
pros::ADIDigitalIn decrease('E');

void pong_screen_task() {
  bool competition_status = false;
  while (true) {
    if (display.enabled()) {
      if (pros::competition::is_connected() || pros::competition::is_autonomous())
        competition_status = true;
      else
        competition_status = false;

      if (display.auton_button_right_new()) {
        if (display.auton_page_current_get() == display.auton_amount_get() - 1 && !display.pong_enabled() && !competition_status) {
          display.pong_enable();
        } else {
          display.pong_disable();
          display.auton_page_up();
        }
      } else if (display.auton_button_left_new()) {
        if (display.auton_page_current_get() == 0 && !display.pong_enabled() && !competition_status) {
          display.pong_enable();
        } else {
          display.pong_disable();
          display.auton_page_down();
        }
      }

      if (competition_status && display.pong_enabled()) {
        display.pong_disable();
        display.auton_page_up();
      }
    }

    pros::delay(20);
  }
}
pros::Task pongScreenTask(pong_screen_task);

void initialize() {
  pros::delay(300);

  display.auton_button_limitswitch_initialize(&increase, &decrease);
  display.enable();
  display.auton_disable();
  display.auton_print();
}
void disabled() {}
void competition_initialize() {}

void autonomous() {
  display.auton_call();
}

void opcontrol() {
  pros::Controller master(CONTROLLER_MASTER);
  while (true) {
    display.pong_loop(master.get_analog(ANALOG_LEFT_Y), master.get_analog(ANALOG_RIGHT_Y));

    pros::delay(20);
  }
}