/* Copyright 2016 Tony Narlock. All rights reserved. */
#include "components/geometry.h"
#include "systems/geometry.h"

GeometrySystem::GeometrySystem() {}

void GeometrySystem::update(entityx::EntityManager& entities,
                        entityx::EventManager& events,
                        entityx::TimeDelta dt) {
  entities.each<Geometry>([dt](entityx::Entity entity, Geometry& geo) {
    geo.position += geo.direction;
  });
}
