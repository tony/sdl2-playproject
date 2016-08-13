/* Copyright 2016 Tony Narlock. All rights reserved. */
#include "bullet.h"
#include "stage.h"
#include "config.h"

LevelStage::LevelStage(const std::unique_ptr<SDL2pp::Renderer>& renderer,
                       const std::unique_ptr<ResourceManager>& resource_manager,
                       const std::shared_ptr<StatService>& stat_service,
                       const std::shared_ptr<spdlog::logger>& console)
    : renderer(renderer),
      bg_texture(resource_manager->GetTexture("bg1")),
      resource_manager(resource_manager),
      console(console) {
  game_panel = std::make_shared<GamePanel>(stat_service, renderer,
                                           resource_manager, console);
  ship = std::make_shared<Ship>(renderer, resource_manager);
  stat_service->set_ship_stats(ship->stats);
}

void LevelStage::HandleInput(const Uint8* currentKeyStates) {
  ship->HandleInput(currentKeyStates);
}

void LevelStage::SpawnEnemy() {
  auto enemy = std::make_shared<Enemy>(renderer, resource_manager, console,
                                       SDL2pp::Point{-1, 0});
  enemies.push_back(enemy);
}

void LevelStage::Update() {
  Uint32 now = SDL_GetTicks();
  if (now - last_bg_scroll >= 150) {
    bg_x_scroll++;
    last_bg_scroll = now;
  }
  renderer->Copy(*bg_texture, SCREEN_RECT + SDL2pp::Point(bg_x_scroll, 0),
                 SDL2pp::NullOpt);

  game_panel->Update();
  ship->Update();

  if (now - last_enemy >= 600) {
    SpawnEnemy();
    last_enemy = now;
  }
  for (auto enemy = enemies.begin(); enemy != enemies.end();) {
    bool enemy_destroyed = false;
    (*enemy)->Update();
    for (auto bullet = ship->bullets.begin(); bullet != ship->bullets.end();) {
      if (SDL2pp::Rect((*enemy)->GetPosition(),
                       (*enemy)->GetSubspriteRect().GetSize())
              .Contains((*bullet)->GetPosition())) {
        (*enemy)->OnHitByBullet(*bullet);
        ship->bullets.erase(bullet);
        if ((*enemy)->stats->health < 1) {
          enemy_destroyed = true;
        }
      } else {
        ++bullet;
      }
    }

    if (enemy_destroyed) {
      enemies.erase(enemy);
    } else {
      ++enemy;
    }
  }
}
