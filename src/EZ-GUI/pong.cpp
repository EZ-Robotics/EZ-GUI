/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "EZ-GUI/gui.hpp"
#include "display/lv_core/lv_obj.h"

namespace ez {

const int SCREEN_WIDTH = 480;
const int SCREEN_HEIGHT = 240;

const std::int16_t
    paddleWidth = 7,
    paddleHeight = 50,
    ballRadius = 5,
    ballVelXInitial = 2,
    ballVelYInitial = 3,
    paddleDistFromEdge = 10,
    ana_stick_sens = 20;
;

const std::uint32_t
    lPaddleColor = 0xFFFFFFFF,
    rPaddleColor = 0xFFFFFFFF,
    ballColor = 0xFFFFFFFF,
    backgroundColor = 0x00000000;

std::uint8_t globalTimer;

std::int16_t
    lPaddleX,
    rPaddleX,
    lPaddleY,
    rPaddleY,
    ballVelX,
    ballVelY,
    ballx,
    bally,
    timer,
    score;
;

void GUI::pong_initialize() {
  if (!pong_initialized) {
    lv_obj_t* temp_left;
    temp_left = lv_obj_create(lv_scr_act(), NULL);
    lv_obj_set_size(temp_left, paddleWidth, paddleHeight);
    lv_obj_set_style(temp_left, &selector_button_style);
    lv_obj_align(temp_left, NULL, LV_ALIGN_IN_TOP_LEFT, lPaddleX, lPaddleY);
    pong_paddle_left = temp_left;

    lv_obj_t* temp_right;
    temp_right = lv_obj_create(lv_scr_act(), NULL);
    lv_obj_set_size(temp_right, paddleWidth, paddleHeight);
    lv_obj_set_style(temp_right, &selector_button_style);
    lv_obj_align(temp_right, NULL, LV_ALIGN_IN_TOP_LEFT, rPaddleX, rPaddleY);
    pong_paddle_right = temp_right;

    lv_obj_t* temp_ball;
    temp_ball = lv_obj_create(lv_scr_act(), NULL);
    lv_obj_set_size(temp_ball, ballRadius * 2, ballRadius * 2);
    lv_obj_set_style(temp_ball, &selector_button_style);
    lv_obj_align(temp_ball, NULL, LV_ALIGN_IN_TOP_LEFT, ballx, bally);
    pong_ball = temp_ball;

    lv_obj_set_hidden(pong_paddle_left, true);
    lv_obj_set_hidden(pong_paddle_right, true);
    lv_obj_set_hidden(pong_ball, true);

    pong_initialized = true;
    is_pong_enabled = false;
  }
}

void GUI::pong_enable() {
  if (!pong_initialized) {
    printf("Pong uninitialized!  Must initialize pong first by running enable()!\n");
    return;
  }

  // Disable motor boxes and selector
  if (!is_pong_enabled) {
    auton_selector_last = auton_enabled;
    auton_disable();
    motor_boxes_hide(true);
  }

  lPaddleX = paddleDistFromEdge;
  rPaddleX = SCREEN_WIDTH - paddleDistFromEdge - paddleWidth;
  lPaddleY = SCREEN_HEIGHT / 2;
  rPaddleY = SCREEN_HEIGHT / 2;
  ballVelX = ballVelXInitial;
  ballVelY = ballVelYInitial;
  ballx = SCREEN_WIDTH / 2;
  bally = SCREEN_HEIGHT / 2;
  timer = 0;
  score = 0;
  pong_score_last = -1;

  lv_obj_set_hidden(pong_paddle_left, false);
  lv_obj_set_hidden(pong_paddle_right, false);
  lv_obj_set_hidden(pong_ball, false);
  is_pong_enabled = true;
}

void GUI::pong_disable() {
  // Disable pong objects
  lv_obj_set_hidden(pong_paddle_left, true);
  lv_obj_set_hidden(pong_paddle_right, true);
  lv_obj_set_hidden(pong_ball, true);

  // Enable motor boxes and reenable selector
  if (motor_boxes_hidden) motor_boxes_hide(false);
  if (auton_selector_last) auton_enable();

  is_pong_enabled = false;
}

bool GUI::pong_enabled() {
  return is_pong_enabled;
}

void GUI::lose_condition() {
  // pros::screen::print(TEXT_MEDIUM_CENTER, 1, "YOU LOST");
  // pros::screen::print(TEXT_MEDIUM_CENTER, 2, "FINAL SCORE: %d", score);
  selector_text_set("GAME OVER!  Final Score: " + std::to_string(score));
  pong_enable();  // reset everything
  ballVelX *= (((globalTimer % 2) * 2) - 1);
  pong_score_last = -1;
  pros::delay(1500);
}

bool GUI::checkCollision(std::int16_t padX,
                         std::int16_t padY,
                         std::int16_t padWidth,
                         std::int16_t padHeight,
                         std::int16_t bX,
                         std::int16_t bY) {
  // check collision
  if ((bY >= padY) && (bY <= (padY + padHeight))) {
    if ((bX >= padX) && (bX <= (padX + padWidth))) {
      return true;
    }
  }
  return false;
}

void GUI::stickMovement(std::int32_t ana_stick_L, std::int32_t ana_stick_R) {
  // move paddles based on sticks
  lPaddleY -= ana_stick_L / ana_stick_sens;
  rPaddleY -= ana_stick_R / ana_stick_sens;

  // bound paddles to the screen size
  lPaddleY = (lPaddleY <= 0 ? 0 : lPaddleY);
  lPaddleY = (lPaddleY >= (SCREEN_HEIGHT - paddleHeight) ? (SCREEN_HEIGHT - paddleHeight) : lPaddleY);

  rPaddleY = (rPaddleY <= 0 ? 0 : rPaddleY);
  rPaddleY = (rPaddleY >= (SCREEN_HEIGHT - paddleHeight) ? (SCREEN_HEIGHT - paddleHeight) : rPaddleY);
}

void GUI::ballMovement() {
  // collision with paddles
  if (checkCollision(lPaddleX + (paddleWidth / 2), lPaddleY, paddleWidth, paddleHeight, ballx, bally)) {
    // collision with left paddle
    if (ballVelX < 0) {
      ballVelX *= -1;
      score++;
      if ((score % 10) == 0) {
        ballVelX *= 1.5;
      }
    }
  }

  if (checkCollision(rPaddleX - paddleWidth, rPaddleY, paddleWidth, paddleHeight, ballx, bally)) {
    // collision with right paddle
    if (ballVelX > 0) {
      ballVelX *= -1;
      score++;
      if ((score % 10) == 0) {
        ballVelX *= 1.5;
      }
    }
  }

  // collision with walls
  if ((bally <= 0) && (ballVelY < 0)) {
    ballVelY *= -1;
    bally = 0;
  }
  if ((bally >= SCREEN_HEIGHT) && (ballVelY > 0)) {
    ballVelY *= -1;
    bally = SCREEN_HEIGHT;
  }
  if ((ballx > (SCREEN_WIDTH + paddleDistFromEdge)) ||
      (ballx < (0 - paddleDistFromEdge))) {
    lose_condition();
  }
  ballx += ballVelX;
  bally += ballVelY;
}

// Update everything
void GUI::pong_update(std::int32_t ana_stick_L, std::int32_t ana_stick_R) {
  stickMovement(ana_stick_L, ana_stick_R);
  ballMovement();
  timer++;
  globalTimer++;
}

// Draw
void GUI::pong_draw() {
  // circleColor(ballx, bally, ballRadius, ballColor);  // BALL
  lv_obj_set_pos(pong_ball, ballx, bally);
  lv_obj_set_pos(pong_paddle_left, lPaddleX, lPaddleY);
  lv_obj_set_pos(pong_paddle_right, rPaddleX, rPaddleY);
  if (score != pong_score_last)
    selector_text_set("Score: " + std::to_string(score));
  pong_score_last = score;
}

// Main loop
void GUI::pong_loop(std::int32_t ana_stick_L, std::int32_t ana_stick_R) {
  if (!is_pong_enabled) return;
  pong_update(ana_stick_L, ana_stick_R);
  pong_draw();
}

}  // namespace ez