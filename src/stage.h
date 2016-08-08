#pragma once

#include "spdlog/spdlog.h"
#include "ship.h"
#include "game_panel.h"
#include <SDL2pp/SDL2pp.hh>

namespace spd = spdlog;

class Stage {
 public:
  Stage(SDL2pp::Renderer& renderer,
        std::shared_ptr<ResourceManager> resource_manager,
        const std::shared_ptr<StatService>& stat_service,
        spd::logger& console);
  void HandleInput(const Uint8* currentKeyStates);
  void Update();

 private:
  SDL2pp::Renderer& renderer;

  std::shared_ptr<Ship> ship;
  std::shared_ptr<GamePanel> game_panel = nullptr;
  SDL2pp::Texture bg_texture = nullptr;
};
