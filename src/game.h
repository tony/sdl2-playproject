#pragma once

#include <memory>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2pp/SDL2pp.hh>
#include <SDL2pp/Window.hh>
#include <SDL2pp/Rect.hh>
#include <SDL2pp/Texture.hh>
#include <SDL2pp/Optional.hh>
#include <SDL2pp/Font.hh>

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
    SDL2pp::Renderer& renderer;
    SDL_Rect position;
    SDL_Point velocity;

    Boomerang(SDL2pp::Renderer& renderer, SDL_Rect position, SDL_Point velocity);
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
    SDL2pp::Texture texture;
};

class Hero {
  public:
    Hero(SDL2pp::Renderer& renderer);
    ~Hero();
    SDL_Rect HeroState[HERO_STATE_TOTAL];
    SDL2pp::Texture spriteSheet;
    SDL_Rect position;
    SDL_Point velocity;
    Stats stats;
    enum HeroState state;
    SDL2pp::Renderer& renderer;
    std::vector<Boomerang*> boomerangs;
    Uint32 last_shot;
    void CreateBoomerang(void);

    void loop(const Uint8* currentKeyStates);
    bool load_textures(void);
};

class GamePanel {
  public:
    GamePanel(Hero* hero, SDL_Renderer* renderer, TTF_Font* font);
    void DrawStats();
    Hero* hero;
    SDL_Renderer* renderer;
    TTF_Font* font;
};

class Game {
  public:
    Game(SDL2pp::Renderer& renderer, SDL2pp::Font& font);
    ~Game();
    void GameLoop();


  private:
    SDL2pp::Renderer& renderer;
    Hero* hero;
    GamePanel* gamepanel;
    SDL_Event e;
    bool quit;
    int imgFlags;
    void SystemLoop(const SDL_Event* e, bool* quit);
    SDL2pp::Window* window;
    SDL2pp::Texture bgTexture;
};

bool game_load_textures(std::shared_ptr<SDL_Texture>& bgTexture,
    SDL_Renderer* renderer);

char* get_full_path(const char* path);

#ifndef __dead
#define __dead __attribute__((__noreturn__))
#endif

/* Fatal errors. */
__dead void fatal(const char* msg, ...);
