/* Copyright 2016 Tony Narlock. All rights reserved. */
#ifndef SRC_SYSTEMS_PLAYER_H_
#define SRC_SYSTEMS_PLAYER_H_
#include "entityx/entityx.h"
#include "SDL2/SDL.h"
#include "input.h"
#include "resource.h"
#include <memory>

// Render all Renderable entities and draw some informational text.
class PlayerSystem : public entityx::System<PlayerSystem> {
 public:
  explicit PlayerSystem(const std::unique_ptr<ResourceManager>& resource_manager);

  virtual void update(entityx::EntityManager& entities,
                      entityx::EventManager& events,
                      entityx::TimeDelta dt) override;

  const Uint8* keys = SDL_GetKeyboardState(nullptr);
  bool spawned = false;
  const std::unique_ptr<ResourceManager>& resource_manager;
};
#endif  // SRC_SYSTEMS_PLAYER_H_
