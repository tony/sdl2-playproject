/* Copyright 2016 Tony Narlock. All rights reserved. */
#ifndef SRC_STAGE_H_
#define SRC_STAGE_H_

#include <spdlog/spdlog.h>
#include "actor.h"
#include "ship.h"
#include "enemy.h"
#include "game_panel.h"
#include <SDL2pp/SDL2pp.hh>
#include <vector>

class Stage {
 public:
  virtual void HandleInput(const Uint8* currentKeyStates) = 0;
  virtual void Update() = 0;
};

class LevelStage : Stage {
 public:
  LevelStage(const std::unique_ptr<SDL2pp::Renderer>& renderer,
             const std::unique_ptr<ResourceManager>& resource_manager,
             const std::shared_ptr<StatService>& stat_service,
             const std::shared_ptr<spdlog::logger>& console);
  void HandleInput(const Uint8* currentKeyStates) final;
  void Update() final;
  void SpawnEnemy();

 private:
  const std::unique_ptr<SDL2pp::Renderer>& renderer;
  const std::shared_ptr<SDL2pp::Texture>& bg_texture;

  const std::unique_ptr<ResourceManager>& resource_manager;
  const std::shared_ptr<spdlog::logger>& console;

  std::shared_ptr<GamePanel> game_panel;
  std::shared_ptr<Ship> ship;
  std::vector<std::shared_ptr<Enemy>> enemies;
  Uint32 last_enemy = 0;
  int bg_x_scroll = 0;
  Uint32 last_bg_scroll = 0;
};
#endif  // SRC_STAGE_H_
