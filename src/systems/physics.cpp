/* Copyright 2016 Tony Narlock. All rights reserved. */
#include "components/geometry.h"
#include "components/collideable.h"
#include "systems/physics.h"
#include "config.h"

PhysicsSystem::PhysicsSystem() {}

void PhysicsSystem::update(entityx::EntityManager& entities,
                           entityx::EventManager& events,
                           entityx::TimeDelta dt) {
  entities.each<Geometry, Collideable>(
      [](entityx::Entity entity, Geometry& geo, Collideable&) {
        if (geo.position.x <= 0) {
          entity.destroy();
        }
      });
}
