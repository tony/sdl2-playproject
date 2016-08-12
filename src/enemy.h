#pragma once

#include <spdlog/spdlog.h>
#include <array>
#include <memory>
#include <vector>
#include "actor.h"
#include "bullet.h"
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
        SDL2pp::Point position,
        SDL2pp::Point velocity = {-1, 0});
  void HandleInput(const Uint8* currentKeyStates) override final;
  void Update() override final;
  void OnHitByBullet(std::shared_ptr<Bullet> bullet);
  std::shared_ptr<EnemyStats> stats;
  std::vector<std::shared_ptr<Bullet>> bullets;

 private:
  const unsigned int shooting_delay = 80;
  void SpawnBullet(void);
  const std::shared_ptr<spdlog::logger>& console;

  Uint32 last_shot = 0;
  bool hit = false;
  Uint32 last_hit = 0;
};
