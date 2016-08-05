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

#define SHIP_MAX_BULLETS 10

extern const int SHIP_SPRITE_W;
extern const int SHIP_SPRITE_H;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

extern const SDL2pp::Rect MAIN_VIEWPORT_RECT;
extern const SDL2pp::Rect BOTTOM_VIEWPORT_RECT;

void draw_text(const char* text,
               const int x,
               const int y,
               SDL2pp::Font& font,
               SDL2pp::Renderer& renderer);

void draw_text(const std::string text,
               const int x,
               const int y,
               SDL2pp::Font& font,
               SDL2pp::Renderer& renderer);

enum ShipState {
  SHIP_STATE_DEFAULT,
  SHIP_STATE_WALK_UP,
  SHIP_STATE_WALK_DOWN,
  SHIP_STATE_WALK_LEFT,
  SHIP_STATE_WALK_RIGHT,
  SHIP_STATE_TOTAL
};

typedef struct Stats {
  int current_hp = 100;
  int hp = 100;
} Stats;

SDL2pp::Texture loadImageAlpha(SDL2pp::Renderer& renderer,
                               std::string spritePath,
                               Uint8 r,
                               Uint8 g,
                               Uint8 b);

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

  SDL2pp::Rect getPosition() const { return position; };
  int getPositionX() const { return position.x; };
  int getPositionY() const { return position.y; };
  SDL2pp::Point getVelocity() const { return velocity; };
  int getVelocityX() const { return velocity.x; };
  int getVelocityY() const { return velocity.y; };

 protected:
  virtual void handleEvents(const Uint8* currentKeyStates) = 0;
  virtual void update(){};

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
  bool inBounds();
  void handleEvents(const Uint8* currentKeyStates) override final;
};

class Ship : public Actor {
 public:
  Ship(SDL2pp::Renderer& renderer,
       SDL2pp::Rect position = {0, 0, 30, 30},
       SDL2pp::Point velocity = {0, 0});
  void handleEvents(const Uint8* currentKeyStates) override final;
  void update() override final;
  Stats stats;

 private:
  void spawnBullet(void);
  std::array<SDL2pp::Rect, ShipState::SHIP_STATE_TOTAL> subsprite;
  enum ShipState state = ShipState::SHIP_STATE_DEFAULT;
  std::vector<Bullet*> bullets;
  Uint32 last_shot = 0;
};

class GamePanel {
 public:
  GamePanel(const std::shared_ptr<Ship>& ship,
            SDL2pp::Renderer& renderer,
            SDL2pp::Font& font);
  void drawStats();
  const std::shared_ptr<Ship>& ship;

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
  void mainLoop();

 private:
  SDL2pp::Renderer& renderer;
  std::shared_ptr<Ship> ship;
  std::shared_ptr<GamePanel> gamepanel;
  SDL_Event e;
  bool quit = false;
  void handleEvent(const SDL_Event* e, bool* quit);
  SDL2pp::Texture bgTexture;
  std::shared_ptr<Input> input;
  spd::logger& console;
};

std::string get_full_path(const char* path);
