#pragma once

#include "spdlog/spdlog.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2pp/Font.hh>
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
class Ship;
class StatService;
struct ShipStats;

class Input {
 public:
  Input();
  const Uint8* keys;
};

class Game {
 public:
  Game(SDL2pp::Renderer& renderer, SDL2pp::Font& font, spd::logger& console);
  ~Game();
  void MainLoop();

 private:
  SDL2pp::Renderer& renderer;
  std::shared_ptr<Ship> ship;
  std::shared_ptr<StatService> stat_service;
  std::shared_ptr<GamePanel> game_panel;
  SDL_Event e;
  bool quit = false;
  void HandleEvent(const SDL_Event* e, bool* quit);
  SDL2pp::Texture bgTexture;
  std::shared_ptr<Input> input;
  spd::logger& console;
};
