/* Copyright 2016 Tony Narlock. All rights reserved. */
#ifndef SRC_GAME_PANEL_H_
#define SRC_GAME_PANEL_H_

#include "spdlog/spdlog.h"
#include <SDL2pp/SDL.hh>
#include <SDL2pp/Font.hh>
#include <memory>
#include "stats.h"
#include "resource.h"

class GamePanel {
 public:
  GamePanel(const std::shared_ptr<StatService>& stat_service,
            const std::unique_ptr<SDL2pp::Renderer>& renderer,
            const std::unique_ptr<ResourceManager>& resource_manager,
            const std::shared_ptr<spdlog::logger>& console);
  void Update();
  const std::shared_ptr<StatService>& stat_service;
  const std::shared_ptr<SDL2pp::Texture>& GetStatsTexture();

 private:
  void DrawStats();
  const std::unique_ptr<SDL2pp::Renderer>& renderer;
  const std::unique_ptr<ResourceManager>& resource_manager;
  std::string last_message_string;
  const std::shared_ptr<spdlog::logger>& console;
};
#endif  // SRC_GAME_PANEL_H_
