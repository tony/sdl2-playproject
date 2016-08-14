/* Copyright 2016 Tony Narlock. All rights reserved. */
#ifndef SRC_ENEMY_H_
#define SRC_ENEMY_H_

#include <spdlog/spdlog.h>
#include <array>
#include <memory>
#include <vector>
#include "actor.h"
#include "bullet.h"
#include "config.h"
#include "resource.h"

typedef struct EnemyStats {
  unsigned int health = 2;
  unsigned int health_max = 2;
  unsigned int level = 1;
} EnemyStats;

class Enemy : public Actor {
 public:
  Enemy(const std::unique_ptr<SDL2pp::Renderer>& renderer,
        const std::unique_ptr<ResourceManager>& resource_manager,
        const std::shared_ptr<spdlog::logger>& console,
        SDL2pp::Point velocity = {-1, 0});
  void Update() final;
  void OnHitByBullet(std::shared_ptr<Bullet> bullet);
  std::shared_ptr<EnemyStats> stats;
  std::vector<std::shared_ptr<Bullet>> bullets;

 private:
  void SpawnBullet(void);
  const std::shared_ptr<spdlog::logger>& console;

  bool hit = false;
  Uint32 last_hit = 0;
};
#endif  // SRC_ENEMY_H_
