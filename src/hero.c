#include <stdbool.h>
#include <SDL.h>
#include "main.h"

extern Hero hero;
extern Boomerangs boomerangs;
extern int SCREEN_HEIGHT;
extern int SCREEN_WIDTH;


void
hero_callback(Hero* hero, Boomerangs *boomerangs, const SDL_Event* e)
{
    if (e->type == SDL_KEYDOWN) {
        switch(e->key.keysym.sym) {
        case SDLK_UP:
            hero->state = HERO_STATE_WALK_UP;
            hero->position.y -= SCREEN_HEIGHT * 0.05;
            break;

        case SDLK_DOWN:
            hero->state = HERO_STATE_WALK_DOWN;
            hero->position.y += SCREEN_HEIGHT * 0.05;
            break;

        case SDLK_LEFT:
            hero->state = HERO_STATE_WALK_LEFT;
            hero->position.x -= SCREEN_WIDTH * 0.05;
            break;

        case SDLK_RIGHT:
            hero->state = HERO_STATE_WALK_RIGHT;
            hero->position.x += SCREEN_WIDTH * 0.05;
            break;

        case SDLK_SPACE:
            boomerang_create(boomerangs, &hero->state, &hero->position);
            break;
        }
    }
}

bool
boomerangs_init(Boomerangs* boomerangs, SDL_Renderer* renderer)
{
    bool success = true;

    boomerangs->len = 0;
    boomerangs->texture = texture_load("resources/boomerang.png", renderer);

    if(boomerangs->texture == NULL) {
        success = false;
    }

    return success;
}

void
boomerangs_update(Boomerangs* boomerangs)
{
    for (int i = 0; i < boomerangs->len; i++) {
        Boomerang* boomerang = &boomerangs->array[i];
        boomerang->position.x += boomerang->velocity.x;
        boomerang->position.y += boomerang->velocity.y;

        if (boomerang->position.x > SCREEN_WIDTH ||
                boomerang->position.x < 0) {

            boomerang_delete(boomerang);
        }
        if (boomerang->position.y > SCREEN_HEIGHT ||
                boomerang->position.y < 0) {
            boomerang_delete(boomerang);
        }
    }
}

void
boomerang_delete(Boomerang* boomerang)
{
    boomerang->position.x = 0;
    boomerang->position.y = 0;
    boomerang->texture = NULL;
    boomerang->velocity.x = 0;
    boomerang->velocity.y = 0;
}

void
boomerangs_draw(Boomerangs* boomerangs, SDL_Renderer* renderer)
{
    for (int i = 0; i < boomerangs->len; i++) {
        Boomerang* boomerang = &boomerangs->array[i];
        SDL_RenderCopy(renderer, boomerang->texture, NULL, &boomerang->position);
    }
}

void
boomerang_create(Boomerangs* boomerangs, const enum HeroState* hero_state, const SDL_Rect* hero_position)
{
    Boomerang boomerang = boomerangs->array[0];
    boomerang.position = *hero_position;
    boomerang.texture = boomerangs->texture;

    switch(*hero_state) {
        case HERO_STATE_WALK_UP:
            boomerang.velocity.x = 0;
            boomerang.velocity.y = -1;
            break;
        case HERO_STATE_WALK_DOWN:
            boomerang.velocity.x = 0;
            boomerang.velocity.y = 1;
            break;
        case HERO_STATE_WALK_LEFT:
            boomerang.velocity.x = -1;
            boomerang.velocity.y = 0;
            break;
        case HERO_STATE_WALK_RIGHT:
            boomerang.velocity.x = 1;
            boomerang.velocity.y = 0;
            break;
        default:
            break;
    }

    boomerangs->array[0] = boomerang;

    boomerangs->len++;
}

bool
hero_load_textures(Hero* hero, SDL_Renderer* renderer)
{
    bool success = true;

    hero->spriteSheet = texture_load("resources/elliot/spritesheet.png", renderer);

    hero->HeroState[HERO_STATE_DEFAULT].x = 0;
    hero->HeroState[HERO_STATE_DEFAULT].y = 0;
    hero->HeroState[HERO_STATE_DEFAULT].w = 30;
    hero->HeroState[HERO_STATE_DEFAULT].h = 30;
    hero->HeroState[HERO_STATE_WALK_UP].x = 0;
    hero->HeroState[HERO_STATE_WALK_UP].y = 1010;
    hero->HeroState[HERO_STATE_WALK_UP].w = 30;
    hero->HeroState[HERO_STATE_WALK_UP].h = 30;
    hero->HeroState[HERO_STATE_WALK_DOWN].x = 0;
    hero->HeroState[HERO_STATE_WALK_DOWN].y = 0;
    hero->HeroState[HERO_STATE_WALK_DOWN].w = 30;
    hero->HeroState[HERO_STATE_WALK_DOWN].h = 30;
    hero->HeroState[HERO_STATE_WALK_LEFT].x = 0;
    hero->HeroState[HERO_STATE_WALK_LEFT].y = 505;
    hero->HeroState[HERO_STATE_WALK_LEFT].w = 30;
    hero->HeroState[HERO_STATE_WALK_LEFT].h = 30;
    hero->HeroState[HERO_STATE_WALK_RIGHT].x = 0;
    hero->HeroState[HERO_STATE_WALK_RIGHT].y = 720;
    hero->HeroState[HERO_STATE_WALK_RIGHT].w = 30;
    hero->HeroState[HERO_STATE_WALK_RIGHT].h = 30;

    if (hero->spriteSheet == NULL) {
        printf("Failed to load hero spritesheet!\n");
        success = false;
    }
    return success;
}

void
hero_delete(Hero* hero)
{
    SDL_DestroyTexture(hero->spriteSheet);
    hero->spriteSheet = NULL;
}
