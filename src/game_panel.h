#pragma once

#include <SDL2pp/SDL.hh>
#include <SDL2pp/Font.hh>
#include <memory>
#include "stats.h"

class GamePanel {
 public:
  GamePanel(const std::shared_ptr<StatService>& stat_service,
            SDL2pp::Renderer& renderer,
            SDL2pp::Font& font);
  void DrawStats();
  const std::shared_ptr<StatService>& stat_service;

 private:
  SDL2pp::Renderer& renderer;
  SDL2pp::Font& font;
};
