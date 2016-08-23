/* Copyright 2016 Tony Narlock. All rights reserved. */
#ifndef SRC_SYSTEMS_GEOMETRY_H_
#define SRC_SYSTEMS_GEOMETRY_H_
#include "entityx/entityx.h"

// Render all Renderable entities and draw some informational text.
struct GeometrySystem : public entityx::System<GeometrySystem> {
 public:
  explicit GeometrySystem();
  virtual void update(entityx::EntityManager& entities,
                      entityx::EventManager& events,
                      entityx::TimeDelta dt) override;
};
#endif  // SRC_SYSTEMS_GEOMETRY_H_
