#pragma once

#define MAX_BOOMERANGS 10
#define HERO_SPRITE_W 30
#define HERO_SPRITE_H 30
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define CLAMP(v, min, max) (MAX(MIN(v, max), min))

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
    SDL_Rect position;
    SDL_Point velocity;
} Boomerang;

typedef struct Boomerangs {
    Boomerang array[MAX_BOOMERANGS];
    int len;
    SDL_Texture* texture;
    Uint32 last_shot;
} Boomerangs;

void hero_callback(Hero* hero, Boomerangs* boomerangs, const Uint8* currentKeyStates);
bool hero_load_textures(Hero* hero, SDL_Renderer* renderer);
void hero_delete(Hero* hero);

bool boomerangs_init(Boomerangs* boomerangs, SDL_Renderer* renderer);
void boomerangs_update(Boomerangs* boomerangs);
void boomerangs_draw(Boomerangs* boomerangs, SDL_Renderer* renderer);
void boomerang_create(Boomerangs* boomerangs, const enum HeroState* hero_state, const SDL_Rect* hero_position);
void boomerang_delete(Boomerang* boomerang);


bool game_load_textures(SDL_Texture** bgTexture, SDL_Renderer* renderer);
void game_close(SDL_Texture* bgTexture, SDL_Renderer* renderer, SDL_Window* window);
void game_callback(const SDL_Event* e, bool* quit);


#ifndef __dead
#define __dead __attribute__ ((__noreturn__))
#endif

/* Fatal errors. */
__dead void fatal(const char *msg, ...);
