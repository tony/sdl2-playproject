#include "game.h"
#include <sstream>
#include "spdlog/spdlog.h"

namespace spd = spdlog;

const int SCREEN_WIDTH = 630;
const int SCREEN_HEIGHT = 480;

const SDL2pp::Rect BOTTOM_VIEWPORT_RECT = {
    0, static_cast<int>(SCREEN_HEIGHT * .9), SCREEN_WIDTH,
    static_cast<int>(SCREEN_HEIGHT * .1)};

const SDL2pp::Rect MAIN_VIEWPORT_RECT = {0, 0, SCREEN_WIDTH,
                                         static_cast<int>(SCREEN_HEIGHT * .9)};

Game::Game(SDL2pp::Renderer& renderer, SDL2pp::Font& font, spd::logger& console)
    : renderer(renderer.SetDrawColor(0xFF, 0xFF, 0xFF, 0xFF)),
      hero(std::make_shared<Hero>(renderer)),
      gamepanel(std::make_shared<GamePanel>(hero, renderer, font)),
      bgTexture(SDL2pp::Texture(renderer, "resources/tiles_12.png")),
      input(std::make_shared<Input>()),
      console(console) {
    console.info("Game started.");
}

Game::~Game() {
  renderer = nullptr;

  IMG_Quit();
  SDL_Quit();
}

void Game::mainLoop() {
  console.info("Game update.");
  while (!quit) {
    renderer.Clear();
    renderer.SetViewport(MAIN_VIEWPORT_RECT);
    renderer.Copy(bgTexture, SDL2pp::NullOpt, SDL2pp::NullOpt);
    if (SDL_PollEvent(&e) != 0) {
      handleEvent(&e, &quit);
    }

    hero->handleEvents(input->keys);
    hero->update();

    gamepanel->drawStats();

    renderer.Present();
    SDL_Delay(16);
  }
}

Input::Input() {
  keys = SDL_GetKeyboardState(nullptr);
}

GamePanel::GamePanel(const std::shared_ptr<Hero>& hero,
                     SDL2pp::Renderer& renderer,
                     SDL2pp::Font& font)
    : hero(hero), renderer(renderer), font(font) {}

void GamePanel::drawStats() {
  std::stringstream herotext;
  renderer.SetViewport(BOTTOM_VIEWPORT_RECT);

  herotext << "health " << hero->stats.current_hp << " / " << hero->stats.hp;
  draw_text(herotext.str(), 0, 0, font, renderer);
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
          console.info("Window {} shown", e->window.windowID);
          break;
        case SDL_WINDOWEVENT_HIDDEN:
          console.info("Window {} hidden", e->window.windowID);
          break;
        case SDL_WINDOWEVENT_EXPOSED:
          console.info("Window {} exposed", e->window.windowID);
          break;
        case SDL_WINDOWEVENT_MOVED:
          console.info("Window {} moved to {},{}", e->window.windowID,
                       e->window.data1, e->window.data2);
          break;
        case SDL_WINDOWEVENT_RESIZED:
          console.info("Window {} resized to {}x{}", e->window.windowID,
                       e->window.data1, e->window.data2);
          break;
        case SDL_WINDOWEVENT_SIZE_CHANGED:
          console.info("Window {} size changed to {}x{}", e->window.windowID,
                       e->window.data1, e->window.data2);
          break;
        case SDL_WINDOWEVENT_MINIMIZED:
          console.info("Window {} minimized", e->window.windowID);
          break;
        case SDL_WINDOWEVENT_MAXIMIZED:
          console.info("Window {} maximized", e->window.windowID);
          break;
        case SDL_WINDOWEVENT_RESTORED:
          console.info("Window {} restored", e->window.windowID);
          break;
        case SDL_WINDOWEVENT_ENTER:
          console.info("Mouse entered window {}", e->window.windowID);
          break;
        case SDL_WINDOWEVENT_LEAVE:
          console.info("Mouse left window {}", e->window.windowID);
          break;
        case SDL_WINDOWEVENT_FOCUS_GAINED:
          console.info("Window {} gained keyboard focus", e->window.windowID);
          break;
        case SDL_WINDOWEVENT_FOCUS_LOST:
          console.info("Window {} lost keyboard focus", e->window.windowID);
          break;
        case SDL_WINDOWEVENT_CLOSE:
          console.info("Window {} closed", e->window.windowID);
          break;
        default:
          console.info("Window {} got unknown event {}", e->window.windowID,
                       e->window.event);
          break;
      }
      break;
  }
}

int main() {
  try {
    // console logger (multithreaded and with color)
    const auto& console(spd::stdout_logger_mt("console", true));
    console->info("Welcome to spdlog!");

    SDL2pp::SDL sdl(SDL_INIT_VIDEO);
    SDL2pp::SDLTTF sdl_ttf;
    SDL2pp::SDLImage image(IMG_INIT_PNG);  // optional

    SDL2pp::Window window("sdl2-playproject", SDL_WINDOWPOS_CENTERED,
                          SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,
                          SDL_WINDOW_RESIZABLE);

    SDL2pp::Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL2pp::Font font("resources/fonts/TerminusTTF-Bold-4.39.ttf", 36);

    Game game(renderer, font, *console);
    game.mainLoop();
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
