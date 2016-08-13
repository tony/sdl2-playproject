/* Copyright 2016 Tony Narlock. All rights reserved. */
#ifndef SRC_SHIP_H_
#define SRC_SHIP_H_

#include <memory>
#include <vector>
#include "actor.h"
#include "bullet.h"
#include "resource.h"

typedef struct ShipStats {
  int health = 100;
  int health_max = 100;
  int level = 1;
} ShipStats;

class Ship : public Actor {
 public:
  Ship(const std::unique_ptr<SDL2pp::Renderer>& renderer,
       const std::unique_ptr<ResourceManager>& resource_manager,
       SDL2pp::Point position = {30, 30},
       SDL2pp::Point velocity = {0, 0});
  void HandleInput(const Uint8* currentKeyStates) final;
  void Update() final;
  std::shared_ptr<ShipStats> stats;
  std::vector<std::shared_ptr<Bullet>> bullets;

 private:
  const unsigned int shooting_delay = 80;
  void SpawnBullet(void);
  Uint32 last_shot = 0;
};
#endif  // SRC_SHIP_H_
