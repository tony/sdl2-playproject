/* Copyright 2016 Tony Narlock. All rights reserved. */
#include "components/renderable.h"
#include "components/geometry.h"
#include "systems/render.h"

RenderSystem::RenderSystem(
    const std::unique_ptr<SDL2pp::Renderer>& renderer,
    const std::unique_ptr<ResourceManager>& resource_manager)
    : renderer(renderer), resource_manager(resource_manager) {}

void RenderSystem::update(entityx::EntityManager& entities,
                          entityx::EventManager& events,
                          entityx::TimeDelta dt) {
  std::ignore = events;
  std::ignore = dt;

  entities.each<Geometry, Renderable>(
      [this](entityx::Entity entity, Geometry& geometry, Renderable& renderable) {
        renderer->Copy(*renderable.texture,
                       SDL2pp::Rect{0, 0, renderable.texture->GetWidth(),
                                    renderable.texture->GetHeight()},
                       geometry.position, 0, SDL2pp::NullOpt, geometry.flip);
      });
}
