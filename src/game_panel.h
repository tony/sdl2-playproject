#pragma once

#include <SDL2pp/SDL.hh>
#include <SDL2pp/Font.hh>
#include <memory>
#include "stats.h"
#include "resource.h"

class GamePanel {
 public:
  GamePanel(const std::shared_ptr<StatService>& stat_service,
            SDL2pp::Renderer& renderer,
            const std::shared_ptr<ResourceManager>& resource_manager);
  void Update();
  const std::shared_ptr<StatService>& stat_service;

 private:
  void DrawStats();
  SDL2pp::Renderer& renderer;
  const std::shared_ptr<ResourceManager>& resource_manager;
};
