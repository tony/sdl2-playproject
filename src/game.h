#pragma once

#include "spdlog/spdlog.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2pp/Optional.hh>
#include <SDL2pp/Rect.hh>
#include <SDL2pp/SDL2pp.hh>
#include <SDL2pp/Texture.hh>
#include <SDL2pp/Surface.hh>
#include <SDL2pp/Window.hh>
#include <memory>
#include <vector>

namespace spd = spdlog;

class Actor;
class GamePanel;
class ResourceManager;
class Ship;
class StatService;
struct ShipStats;

class Game {
 public:
  Game(const std::unique_ptr<SDL2pp::Renderer>& renderer,
       const std::unique_ptr<ResourceManager>& resource_manager,
       spd::logger& console);
  void MainLoop();

 private:
  const std::unique_ptr<SDL2pp::Renderer>& renderer;
  const std::unique_ptr<ResourceManager>& resource_manager;
  std::shared_ptr<StatService> stat_service;
  SDL_Event e;
  bool quit = false;
  void HandleEvent(const SDL_Event* e, bool* quit);
  std::shared_ptr<Input> input;
  spd::logger& console;
};
