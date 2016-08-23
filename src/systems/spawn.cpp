/* Copyright 2016 Tony Narlock. All rights reserved. */
#include "config.h"
#include "util.h"
#include "entityx/entityx.h"
#include "components/geometry.h"
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
  int current_ships = 0;
  entityx::ComponentHandle<Collideable> collideable;
  entities.each<Collideable>(
      [&](entityx::Entity entity, Collideable&) { ++current_ships; });
  for (int i = 0; i < max_ships - current_ships; i++) {
    entityx::Entity entity = entities.create();
    entity.assign<Collideable>(2);
    int x = MAIN_VIEWPORT_RECT.w;
    int y = RandInt() % (MAIN_VIEWPORT_RECT.h - BOTTOM_VIEWPORT_RECT.h);
    auto& sprite = resource_manager->GetTexture("ship1_tinted");
    entity.assign<Geometry>(SDL2pp::Point{x, y}, SDL2pp::Point{-1, RandInt(-1, 1)}, sprite->GetSize());
    entity.assign<Renderable>(sprite);
  }
}
