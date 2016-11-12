/* Copyright 2016 Tony Narlock. All rights reserved. */
#include "player.h"

Player::Player(const std::shared_ptr<LevelStage>& stage) : stage(stage) {}

void Player::HandleInput(const std::shared_ptr<InputManager>& input) {
  std::ignore = input;
}
