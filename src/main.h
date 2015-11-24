#pragma once


SDL_Texture* texture_load(const char* path, SDL_Renderer* renderer);

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
    SDL_Texture* texture;
} Boomerangs;

void hero_callback(Hero* hero, Boomerangs* boomerangs, const SDL_Event* e);
bool hero_load_textures(Hero* hero, SDL_Renderer* renderer);
void hero_delete(Hero* hero);

bool boomerangs_init(Boomerangs* boomerangs, SDL_Renderer* renderer);
void boomerangs_update(Boomerangs* boomerangs);
void boomerangs_draw(Boomerangs* boomerangs, SDL_Renderer* renderer);
void boomerang_create(Boomerangs* boomerangs, const enum HeroState* hero_state, const SDL_Rect* hero_position);
void boomerang_delete(Boomerang* boomerang);


/**
 * Namespace for global objects (window, renderer, etc.)
 */
typedef struct App {
    SDL_Window* window;
    Hero* hero;
    SDL_Renderer* renderer;
    SDL_Texture* bgTexture;
} App;

bool app_load_textures(App* app, SDL_Renderer* renderer);
void app_close(App* app);
void app_callback(App* app, const SDL_Event* e, bool* quit);


#ifndef __dead
#define __dead __attribute__ ((__noreturn__))
#endif

/* Fatal errors. */
__dead void fatal(const char *msg, ...);
