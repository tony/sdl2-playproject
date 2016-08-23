/* Copyright 2016 Tony Narlock. All rights reserved. */
#include "components/collideable.h"
#include "components/geometry.h"
#include "systems/collision.h"

// Determines if two Collideable bodies have collided. If they have it emits a
// CollisionEvent. This is used by ExplosionSystem to create explosion
// particles, but it could be used by a SoundSystem to play an explosion
// sound, etc..
//
// Uses a fairly rudimentary 2D partition system, but performs reasonably well.
CollisionSystem::CollisionSystem(
    const std::shared_ptr<SDL2pp::Renderer>& renderer)
    : size(renderer->GetLogicalSize()) {}

void CollisionSystem::update(entityx::EntityManager& es,
                             entityx::EventManager& events,
                             entityx::TimeDelta dt) {
  reset();
  collect(es);
  collide(events);
};

void CollisionSystem::reset() {
  candidates.empty();
}

void CollisionSystem::collect(entityx::EntityManager& entities) {
  entities.each<Geometry, Collideable>([this](entityx::Entity entity, Geometry& geo,
                                          Collideable& collideable) {
    
    Candidate candidate{geo.position, geo.size, SDL2pp::Rect{geo.position.x, geo.position.y, geo.size.x, geo.size.y}, collideable.radius, entity};
    candidates.push_back(candidate);
  });
}

void CollisionSystem::collide(entityx::EventManager& events) {
    for (const CollisionSystem::Candidate& left : candidates) {
      for (const CollisionSystem::Candidate& right : candidates) {
        if (left.entity == right.entity)
          continue;
        if (collided(left, right))
          std::cout << "HIT" << std::endl;
          events.emit<CollisionEvent>(left.entity, right.entity);
      }
    }
}

bool CollisionSystem::collided(const CollisionSystem::Candidate& left,
                               const CollisionSystem::Candidate& right) {
  return left.area.Contains(right.area);
}
