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
#include <array>
#include <memory>
#include <vector>

namespace spd = spdlog;

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define CLAMP(v, min, max) (MAX(MIN(v, max), min))

#define SHIP_MAX_BULLETS 80

extern const int SHIP_SPRITE_W;
extern const int SHIP_SPRITE_H;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

extern const SDL2pp::Rect MAIN_VIEWPORT_RECT;
extern const SDL2pp::Rect BOTTOM_VIEWPORT_RECT;

void DrawText(const std::string text,
              const int x,
              const int y,
              SDL2pp::Font& font,
              SDL2pp::Renderer& renderer);

SDL2pp::Texture LoadImageAlpha(SDL2pp::Renderer& renderer,
                               std::string spritePath,
                               Uint8 r,
                               Uint8 g,
                               Uint8 b);

enum ShipState {
  SHIP_STATE_DEFAULT,
  SHIP_STATE_WALK_UP,
  SHIP_STATE_WALK_DOWN,
  SHIP_STATE_WALK_LEFT,
  SHIP_STATE_WALK_RIGHT,
  SHIP_STATE_TOTAL
};

typedef struct Stats {
  int health = 100;
  int health_max = 100;
  int level = 1;
} Stats;

class StatService {
 public:
  StatService(int ship_level, int ship_health, int ship_health_max)
      : ship_level(ship_level),
        ship_health(ship_health),
        ship_health_max(ship_health_max) {}
  void set_ship_level(int level) { ship_level = level; };
  void set_ship_health(int health) { ship_health = health; };
  void set_ship_health_max(int health_max) { ship_health_max = health_max; };
  int get_ship_level() const { return ship_level; };
  int get_ship_health() const { return ship_health; };
  int get_ship_health_max() const { return ship_health_max; };

 private:
  int ship_level;
  int ship_health;
  int ship_health_max;
};

class Actor {
 public:
  Actor(SDL2pp::Renderer& renderer,
        SDL2pp::Rect position,
        SDL2pp::Point velocity,
        std::string spritePath)
      : renderer(renderer),
        position(position),
        velocity(velocity),
        sprite(SDL2pp::Texture(renderer, spritePath.c_str())) {}
  Actor(const Actor&) = delete;
  Actor& operator=(const Actor&) = delete;

 protected:
  virtual void HandleEvents(const Uint8* currentKeyStates) = 0;
  virtual void Update(){};

  SDL2pp::Renderer& renderer;
  SDL2pp::Rect position;
  SDL2pp::Point velocity;
  SDL2pp::Texture sprite;
};

class Ship;

class Bullet : public Actor {
 public:
  Bullet(SDL2pp::Renderer& renderer,
         SDL2pp::Rect position,
         SDL2pp::Point velocity);
  bool InBounds();
  void HandleEvents(const Uint8* currentKeyStates) override final;

 private:
  SDL2pp::Point velocity{9, 0};
};

class Ship : public Actor {
 public:
  Ship(SDL2pp::Renderer& renderer,
       SDL2pp::Rect position = {0, 0, 30, 30},
       SDL2pp::Point velocity = {0, 0});
  void HandleEvents(const Uint8* currentKeyStates) override final;
  void Update() override final;
  Stats stats = Stats();

 private:
  const unsigned int shooting_delay = 80;
  void SpawnBullet(void);
  std::array<SDL2pp::Rect, ShipState::SHIP_STATE_TOTAL> subsprite;
  enum ShipState state = ShipState::SHIP_STATE_DEFAULT;
  std::vector<Bullet*> bullets;
  Uint32 last_shot = 0;
};

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

std::string GetFullPath(const char* path);
