/* Copyright 2016 Tony Narlock. All rights reserved. */
#include "components/body.h"
#include "systems/body.h"

BodySystem::BodySystem() {}

void BodySystem::update(entityx::EntityManager& entities,
                        entityx::EventManager& events,
                        entityx::TimeDelta dt) {
  entities.each<Body>([dt](entityx::Entity entity, Body& body) {
    body.position += body.direction;
  });
}
