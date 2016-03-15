#include <iostream>
#include "game.h"

extern const double SCREEN_WIDTH = 630;
extern const double SCREEN_HEIGHT = 480;

extern const SDL2pp::Rect BOTTOM_VIEWPORT_RECT = {
  0, int(SCREEN_HEIGHT * .9), int(SCREEN_WIDTH), int(SCREEN_HEIGHT * .1)};

extern const SDL2pp::Rect MAIN_VIEWPORT_RECT = {0, 0, int(SCREEN_WIDTH),
  int(SCREEN_HEIGHT * .9)};

bool game_load_textures(std::shared_ptr<SDL_Texture>& bgTexture,
    SDL_Renderer* renderer) {
  bool success = true;

  bgTexture = texture_load("resources/tiles_12.png", renderer);
  if (bgTexture == NULL) {
    success = false;
  }


  return success;
}

Game::Game(SDL2pp::Renderer& renderer, SDL2pp::Font& font) : 
  renderer(renderer),
  bgTexture(renderer, get_full_path("resources/tiles_12.png"))
{
  quit = false;
  imgFlags = IMG_INIT_PNG;

  try {

    if (!(IMG_Init(imgFlags) & imgFlags)) {
      fatal("SDL_image could not initialize! SDL_image Error: %s\n",
          IMG_GetError());
    }

    hero = new Hero(renderer);

    if (!hero->load_textures()) {
      fatal("Failed to load hero media!\n");
    } else if (TTF_Init() == -1) {
      fatal("Font engine failed to initialize.\n");
    }


    gamepanel = new GamePanel(hero, renderer.Get(), font.Get());
    renderer.SetDrawColor(0xFF, 0xFF, 0xFF, 0xFF);
  } catch (SDL2pp::Exception& e) {
    // Exception stores SDL_GetError() result and name of function which failed
    std::cerr << "Error in: " << e.GetSDLFunction() << std::endl;
    std::cerr << "  Reason: " << e.GetSDLError() << std::endl;
  } catch (std::exception& e) {
    // This also works (e.g. "SDL_Init failed: No available video device")
    std::cerr << e.what() << std::endl;
  }
}

Game::~Game() {
  // delete font;

  renderer = NULL;
  delete window;

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

void Game::GameLoop() {
  try {
    while (!quit) {
      renderer.Clear();
      renderer.SetViewport(MAIN_VIEWPORT_RECT);
      renderer.Copy(bgTexture, SDL2pp::NullOpt, SDL2pp::NullOpt);
      while (SDL_PollEvent(&e) != 0) {
        SystemLoop(&e, &quit);
      }
      const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
      hero->loop(currentKeyStates);
      renderer.Copy(
          hero->spriteSheet, hero->HeroState[hero->state],
          hero->position);

      hero->boomerangs.erase(
          std::remove_if(hero->boomerangs.begin(), hero->boomerangs.end(),
            [](Boomerang* i) { return i->outOfBounds(); }),
          hero->boomerangs.end());
      for (auto& boomerang : hero->boomerangs) {
        boomerang->loop();
        if (!boomerang->outOfBounds()) {
          boomerang->draw();
        }
      }
      gamepanel->DrawStats();

      renderer.Present();
      SDL_Delay(16);
    }
  } catch (SDL2pp::Exception& e) {
    // Exception stores SDL_GetError() result and name of function which
    // failed
    std::cerr << "Error in: " << e.GetSDLFunction() << std::endl;
    std::cerr << "  Reason: " << e.GetSDLError() << std::endl;
  } catch (std::exception& e) {
    // This also works (e.g. "SDL_Init failed: No available video device")
    std::cerr << e.what() << std::endl;
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
  switch (e->type) {
    case SDL_QUIT:
      *quit = true;
      break;
    case SDL_KEYDOWN:
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
      break;
    case SDL_WINDOWEVENT:
      switch (e->window.event) {
        case SDL_WINDOWEVENT_SHOWN:
          SDL_Log("Window %d shown", e->window.windowID);
          break;
        case SDL_WINDOWEVENT_HIDDEN:
          SDL_Log("Window %d hidden", e->window.windowID);
          break;
        case SDL_WINDOWEVENT_EXPOSED:
          SDL_Log("Window %d exposed", e->window.windowID);
          break;
        case SDL_WINDOWEVENT_MOVED:
          SDL_Log("Window %d moved to %d,%d", e->window.windowID,
              e->window.data1, e->window.data2);
          break;
        case SDL_WINDOWEVENT_RESIZED:
          SDL_Log("Window %d resized to %dx%d", e->window.windowID,
              e->window.data1, e->window.data2);
          break;
        case SDL_WINDOWEVENT_SIZE_CHANGED:
          SDL_Log("Window %d size changed to %dx%d", e->window.windowID,
              e->window.data1, e->window.data2);
          break;
        case SDL_WINDOWEVENT_MINIMIZED:
          SDL_Log("Window %d minimized", e->window.windowID);
          break;
        case SDL_WINDOWEVENT_MAXIMIZED:
          SDL_Log("Window %d maximized", e->window.windowID);
          break;
        case SDL_WINDOWEVENT_RESTORED:
          SDL_Log("Window %d restored", e->window.windowID);
          break;
        case SDL_WINDOWEVENT_ENTER:
          SDL_Log("Mouse entered window %d", e->window.windowID);
          break;
        case SDL_WINDOWEVENT_LEAVE:
          SDL_Log("Mouse left window %d", e->window.windowID);
          break;
        case SDL_WINDOWEVENT_FOCUS_GAINED:
          SDL_Log("Window %d gained keyboard focus", e->window.windowID);
          break;
        case SDL_WINDOWEVENT_FOCUS_LOST:
          SDL_Log("Window %d lost keyboard focus", e->window.windowID);
          break;
        case SDL_WINDOWEVENT_CLOSE:
          SDL_Log("Window %d closed", e->window.windowID);
          break;
        default:
          SDL_Log("Window %d got unknown event %d", e->window.windowID,
              e->window.event);
          break;
      }
      break;
  }
}

int main(void) {
  try {
    SDL2pp::SDL sdl(SDL_INIT_VIDEO);
    SDL2pp::SDLTTF sdl_ttf;
    SDL2pp::SDLImage image(IMG_INIT_PNG); // optional

    SDL2pp::Window window("sdl2-playproject", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_RESIZABLE);

    SDL2pp::Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL2pp::Font font(
        get_full_path("resources/fonts/TerminusTTF-Bold-4.39.ttf"), 36);

    Game game(renderer, font);
    game.GameLoop();
  } catch (SDL2pp::Exception& e) {
    // Exception stores SDL_GetError() result and name of function which failed
    std::cerr << "Error in: " << e.GetSDLFunction() << std::endl;
    std::cerr << "  Reason: " << e.GetSDLError() << std::endl;
  } catch (std::exception& e) {
    // This also works (e.g. "SDL_Init failed: No available video device")
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
