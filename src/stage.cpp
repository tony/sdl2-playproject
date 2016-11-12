/* Copyright 2016 Tony Narlock. All rights reserved. */
#include "bullet.h"
#include "stage.h"
#include "config.h"

LevelStage::LevelStage(const std::unique_ptr<SDL2pp::Renderer>& renderer,
                       const std::unique_ptr<ResourceManager>& resource_manager,
                       const std::shared_ptr<spdlog::logger>& console)
    : renderer(renderer),
      resource_manager(resource_manager),
      console(console),
      bg_texture(resource_manager->GetTextureSheet("bg1")) {}

void LevelStage::HandleInput(const std::shared_ptr<InputManager>& input) {
  player->HandleInput(input);
}

void LevelStage::Update() {
  Uint32 now = SDL_GetTicks();
  if (now - last_bg_scroll >= 150) {
    bg_x_scroll++;
    last_bg_scroll = now;
  }
  if (bg_x_scroll >= bg_texture->GetWidth()) {
    bg_x_scroll = 0;
  }
  renderer->Copy(*bg_texture, SCREEN_RECT + SDL2pp::Point(bg_x_scroll, 0),
                 SDL2pp::NullOpt);

  game_panel->Update();
}
