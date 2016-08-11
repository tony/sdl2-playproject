#pragma once

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
  void HandleInput(const Uint8* currentKeyStates) override final;
  void Update() override final;
  void SpawnEnemy();

 private:
  const std::unique_ptr<SDL2pp::Renderer>& renderer;
  const std::shared_ptr<SDL2pp::Texture>& bg_texture;
  std::shared_ptr<Ship> ship;
  const std::unique_ptr<ResourceManager>& resource_manager;
  std::shared_ptr<GamePanel> game_panel = nullptr;
  std::vector<std::shared_ptr<Actor>> enemies;
};
