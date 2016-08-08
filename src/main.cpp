#include "config.h"
#include "input.h"
#include "game.h"
#include "game_panel.h"
#include "resource.h"
#include "stage.h"

namespace spd = spdlog;

Game::Game(SDL2pp::Renderer& renderer,
           std::shared_ptr<ResourceManager> resource_manager,
           spd::logger& console)
    : renderer(renderer.SetDrawColor(0xFF, 0xFF, 0xFF, 0xFF)),
      resource_manager(resource_manager),
      stat_service(std::make_shared<StatService>()),
      input(std::make_shared<Input>()),
      console(console) {
  console.info("Game started.");
  std::ignore = resource_manager;
}

Game::~Game() {
  renderer = nullptr;

  IMG_Quit();
  SDL_Quit();
}

void Game::MainLoop() {
  auto stage = new Stage(renderer, resource_manager, stat_service, console);
  while (!quit) {
    renderer.Clear();
    renderer.SetViewport(SCREEN_RECT);

    if (SDL_PollEvent(&e) != 0) {
      HandleEvent(&e, &quit);
    }

    stage->HandleInput(input->keys);
    stage->Update();

    renderer.Present();
    SDL_Delay(16);
  }
}

void Game::HandleEvent(const SDL_Event* e, bool* quit) {
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
                          SDL_WINDOWPOS_CENTERED, SCREEN_RECT.w, SCREEN_RECT.h,
                          SDL_WINDOW_RESIZABLE);

    SDL2pp::Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

    // add resources
    auto resource_manager = std::make_shared<ResourceManager>();

    SDL2pp::Font font("resources/fonts/TerminusTTF-Bold-4.39.ttf", 18);
    resource_manager->AddFont("terminus-18", font);

    resource_manager->AddSurface("bg1",
                                 "resources/gfx/side-bg/green-mountain.png");
    resource_manager->AddSurfaceWithTransparency(
        "modular_ships", "resources/gfx/modular_ships.png", 13, 107, 178);
    resource_manager->AddSurfaceWithTransparencyAndTint(
        "modular_ships_tinted", "resources/gfx/modular_ships.png", 13, 107,
        178);
    resource_manager->AddSurfaceWithTransparency(
        "bullets1", "resources/gfx/m484BulletCollection1.png", 0, 0, 0);
    resource_manager->AddSurfaceWithTransparencyAndTint(
        "bullets1_tinted", "resources/gfx/m484BulletCollection1.png", 0, 0, 0);

    Game game(renderer, resource_manager, *console);
    game.MainLoop();
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
