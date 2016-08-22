/* Copyright 2016 Tony Narlock. All rights reserved. */
#ifndef SRC_SYSTEM_RENDER_H_
#define SRC_SYSTEM_RENDER_H_
#include "entityx/entityx.h"

// Render all Renderable entities and draw some informational text.
class RenderSystem : public entityx::System<RenderSystem> {
 public:
  explicit RenderSystem(
      const std::unique_ptr<SDL2pp::Renderer>& renderer,
      const std::unique_ptr<ResourceManager>& resource_manager);

  virtual void update(entityx::EntityManager& entities,
                      entityx::EventManager& events,
                      entityx::TimeDelta dt) override;

  const std::unique_ptr<SDL2pp::Renderer>& renderer;
  const std::unique_ptr<ResourceManager>& resource_manager;
};
#endif  // SRC_SYSTEM_RENDER_H_
