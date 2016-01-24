#include <iostream>
#include <assert.h>
#include "game.h"

extern const double SCREEN_WIDTH = 630;
extern const double SCREEN_HEIGHT = 480;

extern const SDL_Rect BOTTOM_VIEWPORT_RECT = {
    0, int(SCREEN_HEIGHT * .9), int(SCREEN_WIDTH), int(SCREEN_HEIGHT * .1)};

extern const SDL_Rect MAIN_VIEWPORT_RECT = {0, 0, int(SCREEN_WIDTH),
                                            int(SCREEN_HEIGHT * .9)};

TTF_Font* font = NULL;

bool game_load_textures(std::shared_ptr<SDL_Texture>& bgTexture,
                        SDL_Renderer* renderer) {
  bool success = true;

  bgTexture = texture_load("resources/continents.png", renderer);
  if (bgTexture == NULL) {
    success = false;
  }

  return success;
}

Game::Game(void) {
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

  hero = new Hero(renderer);

  if (!game_load_textures(bgTexture, renderer)) {
    fatal("Failed to load media!\n");
  } else if (!hero->load_textures()) {
    fatal("Failed to load hero media!\n");
  } else if (TTF_Init() == -1) {
    fatal("Font engine failed to initialize.\n");
  }

  // load fonts
  font = TTF_OpenFont("resources/fonts/TerminusTTF-Bold-4.39.ttf", 36);
  if (font == NULL) {
    fatal("Error loading font");
  }
  gamepanel = new GamePanel(hero, renderer, font);
  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

Game::~Game() {
  TTF_CloseFont(font);
  font = NULL;

  renderer = NULL;
  SDL_DestroyRenderer(renderer);
  window = NULL;
  SDL_DestroyWindow(window);

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

void Game::GameLoop() {
  while (!quit) {
    SDL_RenderClear(renderer);
    SDL_RenderSetViewport(renderer, &MAIN_VIEWPORT_RECT);
    SDL_RenderCopy(renderer, bgTexture.get(), NULL, NULL);
    while (SDL_PollEvent(&e) != 0) {
      SystemLoop(&e, &quit);
    }
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    hero->loop(currentKeyStates);
    SDL_RenderCopy(renderer, hero->spriteSheet.get(),
                   &hero->HeroState[hero->state], &hero->position);

    hero->boomerangs.erase(
        std::remove_if(hero->boomerangs.begin(), hero->boomerangs.end(),
                       [](Boomerang* i) { return i->outOfBounds(); }),
        hero->boomerangs.end());
    for (auto& boomerang : hero->boomerangs) {
      boomerang->loop();
      if (!boomerang->outOfBounds()) {
        boomerang->draw();
      }
      SDL_assert(renderer == hero->renderer);
      SDL_assert(renderer == boomerang->renderer);
    }
    gamepanel->DrawStats();

    SDL_RenderPresent(renderer);
    SDL_Delay(16);
  }
}

GamePanel::GamePanel(Hero* hero, SDL_Renderer* renderer, TTF_Font* font)
    : hero(hero), renderer(renderer), font(font) {}

void GamePanel::DrawStats() {
  char herotext[32];
  SDL_RenderSetViewport(renderer, &BOTTOM_VIEWPORT_RECT);

  snprintf(herotext, sizeof(herotext), "health %d / %d", hero->stats.current_hp,
           hero->stats.hp);
  draw_text(herotext, 0, 0, font, renderer);
  SDL_RenderSetViewport(renderer, &MAIN_VIEWPORT_RECT);
}

void Game::SystemLoop(const SDL_Event* e, bool* quit) {
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

int main(void) {
  Game game;
  game.GameLoop();
  return 0;
}
