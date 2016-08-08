#pragma once

#include <array>
#include <memory>
#include <vector>
#include "actor.h"
#include "bullet.h"
#include "resource.h"

enum ShipState { DEFAULT, UP, DOWN, LEFT, RIGHT, TOTAL };

typedef struct ShipStats {
  int health = 100;
  int health_max = 100;
  int level = 1;
} ShipStats;

class Ship : public Actor {
 public:
  Ship(const std::shared_ptr<SDL2pp::Renderer>& renderer,
       const std::shared_ptr<ResourceManager>& resource_manager,
       SDL2pp::Rect position = {0, 0, 30, 30},
       SDL2pp::Point velocity = {0, 0});
  void HandleInput(const Uint8* currentKeyStates) override final;
  void Update() override final;
  std::shared_ptr<ShipStats> stats;

 private:
  const unsigned int shooting_delay = 80;
  void SpawnBullet(void);
  std::array<SDL2pp::Rect, ShipState::TOTAL> subsprites;
  enum ShipState state = ShipState::DEFAULT;
  std::vector<Bullet*> bullets;
  Uint32 last_shot = 0;
};
