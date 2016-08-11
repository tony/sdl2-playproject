#pragma once

#include <array>
#include <memory>
#include <vector>
#include "actor.h"
#include "bullet.h"
#include "resource.h"

enum class EnemyState { DEFAULT, UP, DOWN, LEFT, RIGHT, TOTAL };

typedef struct EnemyStats {
  int health = 100;
  int health_max = 100;
  int level = 1;
} EnemyStats;

class Enemy : public Actor {
 public:
  Enemy(const std::unique_ptr<SDL2pp::Renderer>& renderer,
        const std::unique_ptr<ResourceManager>& resource_manager,
        SDL2pp::Point position,
        SDL2pp::Point velocity = {-1, 0});
  void HandleInput(const Uint8* currentKeyStates) override final;
  void Update() override final;
  std::shared_ptr<EnemyStats> stats;

 private:
  const unsigned int shooting_delay = 80;
  void SpawnBullet(void);
  std::array<SDL2pp::Rect, (int)EnemyState::TOTAL> subsprites;
  EnemyState state = EnemyState::DEFAULT;
  std::vector<std::shared_ptr<Bullet>> bullets;
  Uint32 last_shot = 0;
};