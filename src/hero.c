#include <stdbool.h>
#include <SDL.h>
#include "main.h"

extern const int SCREEN_HEIGHT;
extern const int SCREEN_WIDTH;
extern const int SHOOTING_DELAY;

void
hero_callback(Hero* hero, Boomerangs *boomerangs, const Uint8* currentKeyStates)
{
    if (currentKeyStates[SDL_SCANCODE_UP] || currentKeyStates[SDL_SCANCODE_W]) {
        hero->state = HERO_STATE_WALK_UP;
        hero->position.y = CLAMP(hero->position.y - MAIN_VIEWPORT_RECT.h * 0.01, 0, MAIN_VIEWPORT_RECT.h - HERO_SPRITE_H);
    }

    if (currentKeyStates[SDL_SCANCODE_DOWN] || currentKeyStates[SDL_SCANCODE_S]) {
        hero->state = HERO_STATE_WALK_DOWN;
        hero->position.y = CLAMP(hero->position.y + MAIN_VIEWPORT_RECT.h * 0.01, 0, MAIN_VIEWPORT_RECT.h - HERO_SPRITE_H);
    }

    if (currentKeyStates[SDL_SCANCODE_LEFT] || currentKeyStates[SDL_SCANCODE_A]) {
        hero->state = HERO_STATE_WALK_LEFT;
        hero->position.x = CLAMP(hero->position.x - SCREEN_WIDTH * 0.01, 0, SCREEN_WIDTH - HERO_SPRITE_W);
    }

    if (currentKeyStates[SDL_SCANCODE_RIGHT] || currentKeyStates[SDL_SCANCODE_D]) {
        hero->state = HERO_STATE_WALK_RIGHT;
        hero->position.x = CLAMP(hero->position.x + SCREEN_WIDTH * 0.01, 0, SCREEN_WIDTH - HERO_SPRITE_W);
    }

    if (currentKeyStates[SDL_SCANCODE_SPACE]) {
        Uint32 now = SDL_GetTicks();
        if (now - boomerangs->last_shot >= SHOOTING_DELAY) {
            boomerang_create(boomerangs, &hero->state, &hero->position);
            boomerangs->last_shot = now;
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
    int boomerangs_deleted = 0;
    for (int i = 0; i < boomerangs->len; i++) {
        Boomerang* boomerang = &boomerangs->array[i];
		SDL_Rect* position = &boomerang->position;
        position->x += boomerang->velocity.x;
        position->y += boomerang->velocity.y;

        if (position->x > MAIN_VIEWPORT_RECT.w ||
            position->w > MAIN_VIEWPORT_RECT.w ||
            position->x < 0 ||
            position->y > MAIN_VIEWPORT_RECT.h ||
            position->h > MAIN_VIEWPORT_RECT.h ||
            position->y < 0
            ) {
            int remaining_len = boomerangs->len - i - 1;
            memmove(boomerangs->array + i, boomerangs->array + i + 1, remaining_len * sizeof(Boomerang));
            i -= 1;
            boomerangs->len -= 1;
        }
    }
}

void
boomerangs_draw(Boomerangs* boomerangs, SDL_Renderer* renderer)
{
    for (int i = 0; i < boomerangs->len; i++) {
        Boomerang* boomerang = &boomerangs->array[i];

        if (boomerangs->texture) {
            SDL_RenderCopy(renderer, boomerangs->texture, NULL, &boomerang->position);
        }
    }
}

void
boomerang_create(Boomerangs* boomerangs, const enum HeroState* hero_state, const SDL_Rect* hero_position)
{
    if (boomerangs->len < HERO_MAX_BOOMERANGS) {
        Boomerang boomerang = boomerangs->array[boomerangs->len];
        boomerang.position = *hero_position;

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
        boomerangs->array[boomerangs->len] = boomerang;
        boomerangs->len += 1;
    }
}

bool
hero_load_textures(Hero* hero, SDL_Renderer* renderer)
{
    bool success = true;

    hero->spriteSheet = texture_load("resources/elliot/spritesheet.png", renderer);

    hero->HeroState[HERO_STATE_DEFAULT].x = 0;
    hero->HeroState[HERO_STATE_DEFAULT].y = 0;
    hero->HeroState[HERO_STATE_DEFAULT].w = HERO_SPRITE_W;
    hero->HeroState[HERO_STATE_DEFAULT].h = HERO_SPRITE_H;
    hero->HeroState[HERO_STATE_WALK_UP].x = 0;
    hero->HeroState[HERO_STATE_WALK_UP].y = 1010;
    hero->HeroState[HERO_STATE_WALK_UP].w = HERO_SPRITE_W;
    hero->HeroState[HERO_STATE_WALK_UP].h = HERO_SPRITE_H;
    hero->HeroState[HERO_STATE_WALK_DOWN].x = 0;
    hero->HeroState[HERO_STATE_WALK_DOWN].y = 0;
    hero->HeroState[HERO_STATE_WALK_DOWN].w = HERO_SPRITE_W;
    hero->HeroState[HERO_STATE_WALK_DOWN].h = HERO_SPRITE_H;
    hero->HeroState[HERO_STATE_WALK_LEFT].x = 0;
    hero->HeroState[HERO_STATE_WALK_LEFT].y = 505;
    hero->HeroState[HERO_STATE_WALK_LEFT].w = HERO_SPRITE_W;
    hero->HeroState[HERO_STATE_WALK_LEFT].h = HERO_SPRITE_H;
    hero->HeroState[HERO_STATE_WALK_RIGHT].x = 0;
    hero->HeroState[HERO_STATE_WALK_RIGHT].y = 720;
    hero->HeroState[HERO_STATE_WALK_RIGHT].w = HERO_SPRITE_W;
    hero->HeroState[HERO_STATE_WALK_RIGHT].h = HERO_SPRITE_H;

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
