/* Copyright 2016 Tony Narlock. All rights reserved. */
#ifndef SRC_SYSTEMS_SPAWN_H_
#define SRC_SYSTEMS_SPAWN_H_
#include "entityx/entityx.h"

// Render all Renderable entities and draw some informational text.
class SpawnSystem : public entityx::System<SpawnSystem> {
 public:
  explicit SpawnSystem(
      const std::unique_ptr<SDL2pp::Renderer>& renderer,
      const std::unique_ptr<ResourceManager>& resource_manager);

  virtual void update(entityx::EntityManager& entities,
                      entityx::EventManager& events,
                      entityx::TimeDelta dt) override;

  const std::unique_ptr<SDL2pp::Renderer>& renderer;
  const std::unique_ptr<ResourceManager>& resource_manager;
 private:
  int max_ships = 10;
};
#endif  // SRC_SYSTEMS_SPAWN_H_
