/* Copyright 2016 Tony Narlock. All rights reserved. */
#include <fstream>
#include "config.h"
#include "input.h"
#include "game.h"
#include "game_panel.h"
#include "stage.h"
#include "json.hpp"

using json = nlohmann::json;

SDL_Color TintToSDL_Color(json::iterator o) {
  std::array<uint8_t, 4> a;
  int idx = 0;
  for (json::iterator i = o->begin(); i != o->end(); ++i) {
    a[idx] = i->get<uint8_t>();
    idx++;
  }
  return SDL_Color{a[0], a[1], a[2], a[3]};
}

void LoadResources(const std::unique_ptr<SDL2pp::Renderer>& renderer,
                   const std::unique_ptr<ResourceManager>& resource_manager) {
  std::ifstream ifs("resources/manifests/fonts.json");
  json j(ifs);

  for (auto& f : j) {
    if (f.count("name") && f.count("location") && f.count("size")) {
      resource_manager->AddFont(f.find("name").value(),
                                f.find("location").value(),
                                f.find("size").value());
    }
  }

  std::ifstream ifs2("resources/manifests/spritesheets.json");
  json j2(ifs2);

  for (auto& f : j2) {
    if (f.count("name") && f.count("location")) {
      if (f.count("alpha") && f.count("tint")) {
        resource_manager->AddSurfaceWithTransparencyAndTint(
            f.find("name").value(), f.find("location").value(),
            TintToSDL_Color(f.find("alpha")), TintToSDL_Color(f.find("tint")));
      } else if (f.count("alpha")) {
        resource_manager->AddSurfaceWithTransparency(
            f.find("name").value(), f.find("location").value(),
            TintToSDL_Color(f.find("alpha")));
      } else {
        resource_manager->AddSurface(f.find("name").value(),
                                     f.find("location").value());
      }
    }
    resource_manager->AddTextureSheet(
        f.find("name").value(),
        SDL2pp::Texture(*renderer,
                        *resource_manager->GetSurface(f.find("name").value())));
  }
}

Game::Game(const std::shared_ptr<spdlog::logger>& console)
    : sdl(SDL_INIT_VIDEO),
      image(IMG_INIT_PNG),
      window("sdl2-playproject",
             SDL_WINDOWPOS_CENTERED,
             SDL_WINDOWPOS_CENTERED,
             SCREEN_RECT.w,
             SCREEN_RECT.h,
             SDL_WINDOW_RESIZABLE),
      renderer(std::make_unique<SDL2pp::Renderer>(
          window,
          -1,
          SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE)),
      resource_manager(std::make_unique<ResourceManager>()),
      stat_service(std::make_shared<StatService>()),
      input(std::make_shared<Input>()),
      console(console) {
  console->info("Game started.");

  renderer->SetDrawBlendMode(SDL_BLENDMODE_BLEND);
  LoadResources(renderer, resource_manager);
}

void Game::MainLoop() {
  auto stage = std::make_unique<LevelStage>(renderer, resource_manager,
                                            stat_service, console);
  while (!quit) {
    renderer->Clear();
    renderer->SetViewport(SCREEN_RECT);

    if (SDL_PollEvent(&e) != 0) {
      HandleEvent(&e, &quit);
    }

    stage->HandleInput(input->keys);
    stage->Update();

    renderer->Present();
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
          console->info("Window {} shown", e->window.windowID);
          break;
        case SDL_WINDOWEVENT_HIDDEN:
          console->info("Window {} hidden", e->window.windowID);
          break;
        case SDL_WINDOWEVENT_EXPOSED:
          console->info("Window {} exposed", e->window.windowID);
          break;
        case SDL_WINDOWEVENT_MOVED:
          console->info("Window {} moved to {},{}", e->window.windowID,
                        e->window.data1, e->window.data2);
          break;
        case SDL_WINDOWEVENT_RESIZED:
          console->info("Window {} resized to {}x{}", e->window.windowID,
                        e->window.data1, e->window.data2);
          break;
        case SDL_WINDOWEVENT_SIZE_CHANGED:
          console->info("Window {} size changed to {}x{}", e->window.windowID,
                        e->window.data1, e->window.data2);
          break;
        case SDL_WINDOWEVENT_MINIMIZED:
          console->info("Window {} minimized", e->window.windowID);
          break;
        case SDL_WINDOWEVENT_MAXIMIZED:
          console->info("Window {} maximized", e->window.windowID);
          break;
        case SDL_WINDOWEVENT_RESTORED:
          console->info("Window {} restored", e->window.windowID);
          break;
        case SDL_WINDOWEVENT_ENTER:
          console->info("Mouse entered window {}", e->window.windowID);
          break;
        case SDL_WINDOWEVENT_LEAVE:
          console->info("Mouse left window {}", e->window.windowID);
          break;
        case SDL_WINDOWEVENT_FOCUS_GAINED:
          console->info("Window {} gained keyboard focus", e->window.windowID);
          break;
        case SDL_WINDOWEVENT_FOCUS_LOST:
          console->info("Window {} lost keyboard focus", e->window.windowID);
          break;
        case SDL_WINDOWEVENT_CLOSE:
          console->info("Window {} closed", e->window.windowID);
          break;
        default:
          console->info("Window {} got unknown event {}", e->window.windowID,
                        e->window.event);
          break;
      }
      break;
  }
}

int main() {
  try {
    // console logger (multithreaded and with color)
    const std::shared_ptr<spdlog::logger>& console(
        spdlog::stdout_logger_mt("console", true));
    console->info("logger ready.");

    auto game = std::make_unique<Game>(console);
    game->MainLoop();
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
