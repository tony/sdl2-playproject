#include "game.h"
#include <iostream>
#include <sstream>

const int SCREEN_WIDTH = 630;
const int SCREEN_HEIGHT = 480;

const SDL2pp::Rect BOTTOM_VIEWPORT_RECT = {
    0, static_cast<int>(SCREEN_HEIGHT * .9), SCREEN_WIDTH,
    static_cast<int>(SCREEN_HEIGHT * .1)};

const SDL2pp::Rect MAIN_VIEWPORT_RECT = {0, 0, SCREEN_WIDTH,
                                         static_cast<int>(SCREEN_HEIGHT * .9)};

Game::Game(SDL2pp::Renderer& renderer, SDL2pp::Font& font)
    : renderer(renderer),
      hero(std::make_shared<Hero>(renderer)),
      gamepanel(std::make_shared<GamePanel>(hero, renderer, font.Get())),
      bgTexture(nullptr) {
  try {
    bgTexture = SDL2pp::Texture(renderer, "resources/tiles_12.png");

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
  renderer = nullptr;
  window = nullptr;

  IMG_Quit();
  SDL_Quit();
}

void Game::update() {
  while (!quit) {
    renderer.Clear();
    renderer.SetViewport(MAIN_VIEWPORT_RECT);
    renderer.Copy(bgTexture, SDL2pp::NullOpt, SDL2pp::NullOpt);
    if (SDL_PollEvent(&e) != 0) {
      handleEvent(&e, &quit);
    }
    const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);
    hero->handleEvents(currentKeyStates);
    renderer.Copy(hero->spriteSheet, hero->HeroState[hero->state],
                  hero->position);

    gamepanel->drawStats();

    renderer.Present();
    SDL_Delay(16);
  }
}

GamePanel::GamePanel(const std::shared_ptr<Hero>& hero,
                     SDL2pp::Renderer& renderer,
                     TTF_Font* font)
    : hero(hero), renderer(renderer), font(font) {}

void GamePanel::drawStats() {
  std::stringstream herotext;
  renderer.SetViewport(BOTTOM_VIEWPORT_RECT);

  herotext << "health " << hero->stats.current_hp << " / " << hero->stats.hp;
  draw_text(herotext.str().c_str(), 0, 0, font, renderer);
  renderer.SetViewport(MAIN_VIEWPORT_RECT);
}

void Game::handleEvent(const SDL_Event* e, bool* quit) {
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
          if ((e->key.keysym.mod & KMOD_CTRL) != 0) {
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

int main() {
  try {
    SDL2pp::SDL sdl(SDL_INIT_VIDEO);
    SDL2pp::SDLTTF sdl_ttf;
    SDL2pp::SDLImage image(IMG_INIT_PNG);  // optional

    SDL2pp::Window window("sdl2-playproject", SDL_WINDOWPOS_CENTERED,
                          SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,
                          SDL_WINDOW_RESIZABLE);

    SDL2pp::Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL2pp::Font font("resources/fonts/TerminusTTF-Bold-4.39.ttf", 36);

    Game game(renderer, font);
    game.update();
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
