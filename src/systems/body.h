/* Copyright 2016 Tony Narlock. All rights reserved. */
#ifndef SRC_SYSTEMS_BODY_H_
#define SRC_SYSTEMS_BODY_H_
#include "entityx/entityx.h"

// Render all Renderable entities and draw some informational text.
struct BodySystem : public entityx::System<BodySystem> {
 public:
  explicit BodySystem();
  virtual void update(entityx::EntityManager& entities,
                      entityx::EventManager& events,
                      entityx::TimeDelta dt) override;
};
#endif  // SRC_SYSTEMS_BODY_H_
