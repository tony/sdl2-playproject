#pragma once

#include <SDL2pp/SDL2pp.hh>
#include "resource.h"

class Actor {
 public:
  Actor(const std::unique_ptr<SDL2pp::Renderer>& renderer,
        const std::unique_ptr<ResourceManager>& resource_manager,
        SDL2pp::Rect position,
        SDL2pp::Point velocity,
        SDL2pp::Rect subsprite_rect,
        const std::shared_ptr<SDL2pp::Texture>& sprite,
        const std::shared_ptr<SDL2pp::Texture>& shadow)
      : renderer(renderer),
        resource_manager(resource_manager),
        position(position),
        velocity(velocity),
        subsprite_rect(subsprite_rect),
        sprite(sprite),
        shadow(shadow) {}
  Actor(const Actor&) = delete;
  Actor& operator=(const Actor&) = delete;

 protected:
  virtual void HandleInput(const Uint8* currentKeyStates) {
    std::ignore = currentKeyStates;
  }
  virtual void Update(){};

  const std::unique_ptr<SDL2pp::Renderer>& renderer;
  const std::unique_ptr<ResourceManager>& resource_manager;
  SDL2pp::Rect position;
  SDL2pp::Point velocity;
  SDL2pp::Rect subsprite_rect;
  const std::shared_ptr<SDL2pp::Texture>& sprite;
  const std::shared_ptr<SDL2pp::Texture>& shadow;
};
