/* Copyright 2016 Tony Narlock. All rights reserved. */
#ifndef SRC_SYSTEMS_PHYSICS_H_
#define SRC_SYSTEMS_PHYSICS_H_
#include "entityx/entityx.h"

// Render all Renderable entities and draw some informational text.
class PhysicsSystem : public entityx::System<PhysicsSystem> {
 public:
  explicit PhysicsSystem();
  virtual void update(entityx::EntityManager& entities,
                      entityx::EventManager& events,
                      entityx::TimeDelta dt) override;
};
#endif  // SRC_SYSTEMS_PHYSICS_H_
