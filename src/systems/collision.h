/* Copyright 2016 Tony Narlock. All rights reserved. */
#ifndef SRC_SYSTEMS_COLLISION_H_
#define SRC_SYSTEMS_COLLISION_H_
#include "entityx/entityx.h"
#include "components/identity.h"

// Emitted when two entities collide.
struct CollisionEvent {
  CollisionEvent(entityx::Entity left, entityx::Entity right)
      : left(left), right(right) {}

  entityx::Entity left, right;
};

// Render all Renderable entities and draw some informational text.
struct CollisionSystem : public entityx::System<CollisionSystem> {
  static const int PARTITIONS = 200;

  struct Candidate {
    SDL2pp::Point position;
    SDL2pp::Point size;
    SDL2pp::Rect area;
    float radius;
    entityx::Entity entity;
    Identity identity;
  };

 public:
  explicit CollisionSystem();
  virtual void update(entityx::EntityManager& entities,
                      entityx::EventManager& events,
                      entityx::TimeDelta dt) override;

 private:
  std::vector<Candidate> candidates;
};
#endif  // SRC_SYSTEMS_COLLISION_H_
