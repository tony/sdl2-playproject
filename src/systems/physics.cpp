/* Copyright 2016 Tony Narlock. All rights reserved. */
#include "components/body.h"
#include "systems/physics.h"

PhysicsSystem::PhysicsSystem() {}

void PhysicsSystem::update(entityx::EntityManager& entities,
                           entityx::EventManager& events,
                           entityx::TimeDelta dt) {
  entities.each<Body>([dt](entityx::Entity entity, Body& body) {});
}
