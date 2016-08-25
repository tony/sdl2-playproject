/* Copyright 2016 Tony Narlock. All rights reserved. */
#include "config.h"
#include "util.h"
#include "entityx/entityx.h"
#include "components/enemy.h"
#include "components/geometry.h"
#include "components/identity.h"
#include "components/collideable.h"
#include "components/renderable.h"
#include "systems/spawn.h"

SpawnSystem::SpawnSystem(
    const std::unique_ptr<SDL2pp::Renderer>& renderer,
    const std::unique_ptr<ResourceManager>& resource_manager)
    : renderer(renderer), resource_manager(resource_manager) {}

void SpawnSystem::update(entityx::EntityManager& entities,
                         entityx::EventManager& events,
                         entityx::TimeDelta dt) {
  std::ignore = events;
  std::ignore = dt;
  int current_enemies = 0;
  entityx::ComponentHandle<Collideable> collideable;
  entities.each<Collideable>(
      [&](entityx::Entity entity, Collideable&) { ++current_enemies; });
  auto& sprite = resource_manager->GetTexture("ship1_tinted");
  if (current_enemies < max_enemies && dt - last_enemy >= RandInt(600, 800)) {
    int x = MAIN_VIEWPORT_RECT.w;
    int y = RandInt() % (MAIN_VIEWPORT_RECT.h - BOTTOM_VIEWPORT_RECT.h);
    entityx::Entity entity = entities.create();
    entity.assign<Collideable>(2);
    entity.assign<Enemy>();
    entity.assign<Geometry>(SDL2pp::Point{x, y}, SDL2pp::Point{-1, 0},
                            sprite->GetSize(), 0, 1, SDL_FLIP_HORIZONTAL);
    entity.assign<Renderable>(sprite);
    entity.assign<Identity>(EntityIdentity::SHIP);
    last_enemy = dt;
  }
}
