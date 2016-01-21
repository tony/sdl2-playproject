#include <assert.h>
#include <stdbool.h>
#include "main.h"

extern const double SCREEN_WIDTH = 630;
extern const double SCREEN_HEIGHT = 480;

extern const SDL_Rect BOTTOM_VIEWPORT_RECT = {
    0, int(SCREEN_HEIGHT * .9), int(SCREEN_WIDTH), int(SCREEN_HEIGHT * .1)};

extern const SDL_Rect MAIN_VIEWPORT_RECT = {0, 0, int(SCREEN_WIDTH),
                                            int(SCREEN_HEIGHT * .9)};

extern const int SHOOTING_DELAY = 200;

const int HERO_SPRITE_W = 30;
const int HERO_SPRITE_H = 30;

TTF_Font* font = NULL;

bool game_load_textures(SDL_Texture** bgTexture, SDL_Renderer* renderer) {
  bool success = true;

  *bgTexture = texture_load("resources/continents.png", renderer);
  if (bgTexture == NULL) {
    success = false;
  }

  return success;
}

void game_close(SDL_Texture* bgTexture,
                SDL_Renderer* renderer,
                SDL_Window* window) {
  SDL_DestroyTexture(bgTexture);
  bgTexture = NULL;
  TTF_CloseFont(font);
  font = NULL;

  renderer = NULL;
  window = NULL;
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

void game_callback(const SDL_Event* e, bool* quit) {
  if (e->type == SDL_QUIT) {
    *quit = true;
  } else if (e->type == SDL_KEYDOWN) {
    switch (e->key.keysym.sym) {
      case SDLK_ESCAPE:
        *quit = true;
        break;

      case SDLK_c:
        if (e->key.keysym.mod & KMOD_CTRL) {
          *quit = true;
        }
        break;
    }
  }
}

Hero::Hero(void) {
  spriteSheet = NULL, position = {0, 0, 30, 30};
  state = HERO_STATE_DEFAULT;
  stats.current_hp = 100;
  stats.hp = 100;
  stats.strength = 8;
  stats.intelligence = 8;
}

GCore::GCore(void) {
  quit = false;
  imgFlags = IMG_INIT_PNG;
  bgTexture = nullptr;
  
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fatal("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
  }

  if (!(IMG_Init(imgFlags) & imgFlags)) {
    fatal("SDL_image could not initialize! SDL_image Error: %s\n",
          IMG_GetError());
  }

  window = SDL_CreateWindow("sdl2-playproject", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,
                            SDL_WINDOW_RESIZABLE);

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if (window == NULL) {
    fatal("Window could not be created!  SDL_Error: %s\n", SDL_GetError());
  } else if (renderer == NULL) {
    fatal("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
  }

  if (!game_load_textures(&bgTexture, renderer)) {
    fatal("Failed to load media!\n");
  } else if (!hero_load_textures(&hero, renderer)) {
    fatal("Failed to load hero media!\n");
  } else if (!boomerangs_init(&boomerangs, renderer)) {
    fatal("Failed to load boomerang media!\n");
  } else if (TTF_Init() == -1) {
    fatal("Font engine failed to initialize.\n");
  }

  // load fonts
  font = TTF_OpenFont("resources/fonts/TerminusTTF-Bold-4.39.ttf", 36);
  if (font == NULL) {
    fatal("Error loading font");
  }

  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

  while (!quit) {
    SDL_RenderClear(renderer);
    SDL_RenderSetViewport(renderer, &MAIN_VIEWPORT_RECT);
    SDL_RenderCopy(renderer, bgTexture, NULL, NULL);
    while (SDL_PollEvent(&e) != 0) {
      game_callback(&e, &quit);
    }
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    hero_callback(&hero, &boomerangs, currentKeyStates);
    boomerangs_update(&boomerangs);
    boomerangs_draw(&boomerangs, renderer);
    SDL_RenderCopy(renderer, hero.spriteSheet, &hero.HeroState[hero.state],
                   &hero.position);
    char herotext[32];
    snprintf(herotext, sizeof(herotext), "health %d / %d",
             hero.stats.current_hp, hero.stats.hp);
    draw_text(herotext, 0, 0, font, renderer);

    SDL_RenderPresent(renderer);
    SDL_Delay(16);
  }
}

GCore::~GCore() {
  hero_delete(&hero);

  SDL_DestroyTexture(bgTexture);
  bgTexture = NULL;
  TTF_CloseFont(font);
  font = NULL;

  renderer = NULL;
  window = NULL;
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

int main(void) {
  GCore gCore;
  return 0;
}
