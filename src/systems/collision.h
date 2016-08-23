/* Copyright 2016 Tony Narlock. All rights reserved. */
#ifndef SRC_SYSTEMS_COLLISION_H_
#define SRC_SYSTEMS_COLLISION_H_
#include "entityx/entityx.h"

// Emitted when two entities collide.
struct CollisionEvent {
  CollisionEvent(entityx::Entity left, entityx::Entity right)
      : left(left), right(right) {
        std::ignore = left;
        std::ignore = right;
      }

  entityx::Entity left, right;
};


// Render all Renderable entities and draw some informational text.
struct CollisionSystem : public entityx::System<CollisionSystem> {
  static const int PARTITIONS = 200;

  struct Candidate {
    SDL2pp::Point position;
    float radius;
    entityx::Entity entity;
  };
 public:
  explicit CollisionSystem(const std::shared_ptr<SDL2pp::Renderer>& renderer);
  virtual void update(entityx::EntityManager& entities,
                      entityx::EventManager& events,
                      entityx::TimeDelta dt) override;
bool collided(const CollisionSystem::Candidate &left, const CollisionSystem::Candidate &right);
float length(const SDL2pp::Point& v);
  void collide(entityx::EventManager &events) ;
  void collect(entityx::EntityManager &entities) ;
  void reset() ;
private:
  std::vector<std::vector<Candidate>> grid;
  SDL2pp::Point size;
};
#endif  // SRC_SYSTEMS_COLLISION_H_
