/* Copyright 2016 Tony Narlock. All rights reserved. */
#include "enemy.h"

#include "config.h"
#include "resource.h"
#include "stage.h"
#include "util.h"

Enemy::Enemy(const std::shared_ptr<LevelStage>& stage,
             SDL2pp::Optional<SDL2pp::Point> position,
             SDL2pp::Point velocity,
             int flip)
    : ship(std::make_shared<Ship>(
          stage,
          std::map<std::string, std::string>{{"default", "ship1_tinted"},
                                             {"hit", "ship1_hit"}},
          position ? position.value() : GenerateSpawnPosition(),
          velocity,
          std::make_shared<ShipStats>(ShipStats{2, 2, 1}),
          flip,
          RandFloat(1, 2))) {
  stage->console->info("spawned new enemy @({},{}) size ({}x{})",
                       ship->position.x, ship->position.y, ship->GetSize().x,
                       ship->GetSize().y);
}

void Enemy::Update(const std::unique_ptr<SDL2pp::Renderer>& renderer) {
  ship->position.x--;
  ship->Update(renderer);
}
