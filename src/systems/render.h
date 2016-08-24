/* Copyright 2016 Tony Narlock. All rights reserved. */
#ifndef SRC_SYSTEMS_RENDER_H_
#define SRC_SYSTEMS_RENDER_H_
#include "entityx/entityx.h"
#include <SDL2pp/Renderer.hh>
#include "resource.h"

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
#endif  // SRC_SYSTEMS_RENDER_H_
