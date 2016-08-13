#ifndef HAVE_GAME_PANEL_H_INCLUDED
#define HAVE_GAME_PANEL_H_INCLUDED

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
#endif  // HAVE_GAME_PANEL_H_INCLUDED
