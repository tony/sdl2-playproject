#pragma once

bool app_load_textures(void);
bool hero_load_textures(void);
void app_close(void);
void app_callback(const SDL_Event* e, bool* quit);
void hero_callback(const SDL_Event* e);
SDL_Texture* texture_load(const char* path);

enum HeroState {
    HERO_STATE_DEFAULT,
    HERO_STATE_WALK_UP,
    HERO_STATE_WALK_DOWN,
    HERO_STATE_WALK_LEFT,
    HERO_STATE_WALK_RIGHT,
    HERO_STATE_TOTAL
};

typedef struct Hero {
    SDL_Rect HeroState[HERO_STATE_TOTAL];
    SDL_Texture* spriteSheet; // sprite sheet
    SDL_Rect position;
    SDL_Point velocity;
    enum HeroState state;
} Hero;

typedef struct Boomerang {
    SDL_Texture* texture;
    SDL_Rect position;
    SDL_Point velocity;
} Boomerang;

typedef struct Boomerangs {
    Boomerang array[10];
    int len;
} Boomerangs;

void boomerangs_init();
void boomerang_create(const enum HeroState* hero_state, const SDL_Rect* hero_position);
void boomerangs_update();
void boomerangs_draw(SDL_Renderer* renderer);


/**
 * Namespace for global objects (window, renderer, etc.)
 */
typedef struct App {
    SDL_Window* window;
    Hero* hero;
    SDL_Renderer* renderer;
    SDL_Texture* bgTexture;
} App;

#ifndef __dead
#define __dead __attribute__ ((__noreturn__))
#endif

/* Fatal errors. */
__dead void fatal(const char *msg, ...);
