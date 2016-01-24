#pragma once

#include <memory>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define CLAMP(v, min, max) (MAX(MIN(v, max), min))

#define HERO_MAX_BOOMERANGS 10

extern const int HERO_SPRITE_W;
extern const int HERO_SPRITE_H;

extern const SDL_Rect MAIN_VIEWPORT_RECT;
extern const SDL_Rect BOTTOM_VIEWPORT_RECT;

std::shared_ptr<SDL_Texture> texture_load(const char* path,
                                          SDL_Renderer* renderer);
void draw_text(const char* text,
               const int x,
               const int y,
               TTF_Font* font,
               SDL_Renderer* renderer);

enum HeroState {
  HERO_STATE_DEFAULT,
  HERO_STATE_WALK_UP,
  HERO_STATE_WALK_DOWN,
  HERO_STATE_WALK_LEFT,
  HERO_STATE_WALK_RIGHT,
  HERO_STATE_TOTAL
};

typedef struct Stats {
  int current_hp;
  int hp;
  int strength;
  int intelligence;
} Stats;

class Boomerang {
 public:
  SDL_Renderer* renderer;
  SDL_Rect position;
  SDL_Point velocity;

  Boomerang(SDL_Renderer* renderer, SDL_Rect position, SDL_Point velocity);
  ~Boomerang();
  void loop();
  SDL_Rect getPosition() const { return position; };
  int getPositionX() const { return position.x; };
  int getPositionY() const { return position.y; };
  SDL_Point getVelocity() const { return velocity; };
  int getVelocityX() const { return velocity.x; };
  int getVelocityY() const { return velocity.y; };
  bool outOfBounds();
  void draw();

 private:
  std::shared_ptr<SDL_Texture> texture;
};

class Hero {
 public:
  Hero(SDL_Renderer* renderer);
  ~Hero();
  SDL_Rect HeroState[HERO_STATE_TOTAL];
  std::shared_ptr<SDL_Texture> spriteSheet;  // sprite sheet
  SDL_Rect position;
  SDL_Point velocity;
  Stats stats;
  enum HeroState state;
  SDL_Renderer* renderer;
  std::vector<Boomerang*> boomerangs;
  Uint32 last_shot;
  void CreateBoomerang(void);

  void loop(const Uint8* currentKeyStates);
  bool load_textures(void);
};

class GCore {
 public:
  GCore(void);
  ~GCore();
  void loop();
  SDL_Renderer* renderer;

 private:
  Hero* hero;
  TTF_Font* font;
  SDL_Event e;
  bool quit;
  int imgFlags;
  void game_loop(const SDL_Event* e, bool* quit);
  SDL_Window* window;
  std::shared_ptr<SDL_Texture> bgTexture;
};

bool game_load_textures(std::shared_ptr<SDL_Texture>& bgTexture,
                        SDL_Renderer* renderer);

#ifndef __dead
#define __dead __attribute__((__noreturn__))
#endif

/* Fatal errors. */
__dead void fatal(const char* msg, ...);
