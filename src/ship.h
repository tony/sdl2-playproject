/* Copyright 2016 Tony Narlock. All rights reserved. */
#ifndef SRC_SHIP_H_
#define SRC_SHIP_H_

#include <spdlog/spdlog.h>
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

class Ship : public Actor, public std::enable_shared_from_this<Ship> {
 public:
  Ship(const std::unique_ptr<SDL2pp::Renderer>& renderer,
       const std::unique_ptr<ResourceManager>& resource_manager,
       const std::shared_ptr<spdlog::logger>& console,
       const std::string& texture_key = "ship1",
       SDL2pp::Optional<SDL2pp::Point> position = SDL2pp::NullOpt,
       SDL2pp::Point velocity = {0, 0},
       const std::shared_ptr<ShipStats>& stats = std::make_shared<ShipStats>(),
       int flip = 0);

  void Update() final;
  void HandleInput(const Uint8* currentKeyStates);
  void OnHitByBullet(std::shared_ptr<Bullet> bullet);
  int GetFlip() const { return flip; }
  void SetHit(int h) { hit = h; }
  std::shared_ptr<ShipGraphicsComponent> graphics_;
  std::shared_ptr<ShipStats> stats;
  std::vector<std::shared_ptr<Bullet>> bullets;
  bool hit = false;
  Uint32 last_hit = 0;

 private:
  const std::shared_ptr<spdlog::logger>& console;

  const unsigned int shooting_delay = 80;
  void SpawnBullet(void);
  Uint32 last_shot = 0;

  int flip = 0;
};

class Player {
 public:
  Player(const std::unique_ptr<SDL2pp::Renderer>& renderer,
         const std::unique_ptr<ResourceManager>& resource_manager,
         const std::shared_ptr<spdlog::logger>& console);

  void HandleInput(const Uint8* currentKeyStates);
  std::shared_ptr<Ship> ship;
};
#endif  // SRC_SHIP_H_
