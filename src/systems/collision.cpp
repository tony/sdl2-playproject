/* Copyright 2016 Tony Narlock. All rights reserved. */
#include "components/collideable.h"
#include "components/body.h"
#include "systems/collision.h"

// Determines if two Collideable bodies have collided. If they have it emits a
// CollisionEvent. This is used by ExplosionSystem to create explosion
// particles, but it could be used by a SoundSystem to play an explosion
// sound, etc..
//
// Uses a fairly rudimentary 2D partition system, but performs reasonably well.
CollisionSystem::CollisionSystem(
    const std::shared_ptr<SDL2pp::Renderer>& renderer)
    : size(renderer->GetLogicalSize()) {
  size.x = size.x / PARTITIONS + 1;
  size.y = size.y / PARTITIONS + 1;
}

void CollisionSystem::update(entityx::EntityManager& es,
                             entityx::EventManager& events,
                             entityx::TimeDelta dt) {
  reset();
  collect(es);
  collide(events);
};

void CollisionSystem::reset() {
  grid.clear();
  grid.resize(size.x * size.y);
}

void CollisionSystem::collect(entityx::EntityManager& entities) {
  entities.each<Body, Collideable>([this](entityx::Entity entity, Body& body,
                                          Collideable& collideable) {
    unsigned int left = static_cast<int>(body.position.x - collideable.radius) /
                        PARTITIONS,
                 top = static_cast<int>(body.position.y - collideable.radius) /
                       PARTITIONS,
                 right =
                     static_cast<int>(body.position.x + collideable.radius) /
                     PARTITIONS,
                 bottom =
                     static_cast<int>(body.position.y + collideable.radius) /
                     PARTITIONS;
    CollisionSystem::Candidate candidate{body.position, collideable.radius,
                                         entity};
    unsigned int slots[4] = {
        left + top * size.x, right + top * size.x, left + bottom * size.x,
        right + bottom * size.x,
    };
    grid[slots[0]].push_back(candidate);
    if (slots[0] != slots[1])
      grid[slots[1]].push_back(candidate);
    if (slots[1] != slots[2])
      grid[slots[2]].push_back(candidate);
    if (slots[2] != slots[3])
      grid[slots[3]].push_back(candidate);
  });
}

void CollisionSystem::collide(entityx::EventManager& events) {
  for (const std::vector<CollisionSystem::Candidate>& candidates : grid) {
    for (const CollisionSystem::Candidate& left : candidates) {
      for (const CollisionSystem::Candidate& right : candidates) {
        if (left.entity == right.entity)
          continue;
        if (collided(left, right))
          events.emit<CollisionEvent>(left.entity, right.entity);
      }
    }
  }
}

float CollisionSystem::length(const SDL2pp::Point& v) {
  return std::sqrt(v.x * v.x + v.y * v.y);
}

bool CollisionSystem::collided(const CollisionSystem::Candidate& left,
                               const CollisionSystem::Candidate& right) {
  return length(left.position - right.position) < left.radius + right.radius;
}
