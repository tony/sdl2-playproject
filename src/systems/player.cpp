/* Copyright 2016 Tony Narlock. All rights reserved. */
#include "entityx/entityx.h"
#include "components/collideable.h"
#include "components/geometry.h"
#include "components/renderable.h"
#include "components/player.h"
#include "systems/player.h"
#include "resource.h"
#include <SDL2pp/SDL2pp.hh>

PlayerSystem::PlayerSystem(const std::unique_ptr<ResourceManager>& resource_manager) : resource_manager(resource_manager) {}

void PlayerSystem::update(entityx::EntityManager& entities,
                            entityx::EventManager& events,
                            entityx::TimeDelta dt) {
  if (!spawned) {
  auto& sprite = resource_manager->GetTexture("ship1");
  entityx::Entity entity = entities.create();
  entity.assign<Collideable>(2);
  entity.assign<Geometry>(SDL2pp::Point{30, 30}, SDL2pp::Point{0, 0},
                          sprite->GetSize());
  entity.assign<Renderable>(sprite);
  entity.assign<Player>();
  spawned = true;
  }

}
