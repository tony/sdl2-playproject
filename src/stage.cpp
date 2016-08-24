/* Copyright 2016 Tony Narlock. All rights reserved. */
#include "bullet.h"
#include "stage.h"
#include "systems/spawn.h"
#include "systems/geometry.h"
#include "systems/physics.h"
#include "systems/player.h"

LevelStage::LevelStage(const std::unique_ptr<SDL2pp::Renderer>& renderer,
                       const std::unique_ptr<ResourceManager>& resource_manager,
                       const std::shared_ptr<StatService>& stat_service,
                       const std::shared_ptr<spdlog::logger>& console)
    : renderer(renderer),
      bg_texture(resource_manager->GetTextureSheet("bg1")),
      resource_manager(resource_manager),
      console(console),
      game_panel(std::make_shared<GamePanel>(stat_service,
                                             renderer,
                                             resource_manager,
                                             console)),
      player(std::make_shared<Player>(renderer, resource_manager, console)) {
  stat_service->set_ship_stats(player->ship->stats);
  systems.add<RenderSystem>(renderer, resource_manager);
  systems.add<SpawnSystem>(renderer, resource_manager);
  systems.add<PlayerSystem>(resource_manager, "ship1");
  systems.add<PhysicsSystem>();
  systems.add<GeometrySystem>();
  systems.configure();
}

void LevelStage::HandleInput(const Uint8* currentKeyStates) {
  player->HandleInput(currentKeyStates);
}

void LevelStage::SpawnEnemy() {
  auto enemy = std::make_shared<Enemy>(renderer, resource_manager, console,
                                       SDL2pp::NullOpt, SDL2pp::Point{-1, 0},
                                       SDL_FLIP_HORIZONTAL);
  enemies.push_back(enemy);
}

void LevelStage::update(entityx::TimeDelta dt) {
  Uint32 now = SDL_GetTicks();
  if (now - last_bg_scroll >= 150) {
    bg_x_scroll++;
    last_bg_scroll = now;
  }
  renderer->Copy(*bg_texture, SCREEN_RECT + SDL2pp::Point(bg_x_scroll, 0),
                 SDL2pp::NullOpt);

  game_panel->Update();
  // player->ship->Update();
  systems.update<RenderSystem>(dt);
  systems.update<SpawnSystem>(dt);
  systems.update<PhysicsSystem>(dt);
  systems.update<GeometrySystem>(dt);
  systems.update<PlayerSystem>(dt);

  if (now - last_enemy >= 600) {
    // SpawnEnemy();
    last_enemy = now;
  }
  for (auto enemy = enemies.begin(); enemy != enemies.end();) {
    bool enemy_destroyed = false;
    (*enemy)->Update();
    for (auto bullet = player->ship->bullets.begin();
         bullet != player->ship->bullets.end();) {
      if ((*enemy)->ship->GetSubspriteRect().Intersects(
              (*bullet)->GetSubspriteRect())) {
        (*enemy)->ship->OnHitByBullet(*bullet);
        player->ship->bullets.erase(bullet);
        if ((*enemy)->ship->stats->health < 1) {
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
