#pragma once

#include <SDL2pp/SDL2pp.hh>
#include "resource.h"

class Actor {
 public:
  Actor(SDL2pp::Renderer& renderer,
        const std::shared_ptr<ResourceManager>& resource_manager,
        SDL2pp::Rect position,
        SDL2pp::Point velocity)
      : renderer(renderer),
        resource_manager(resource_manager),
        position(position),
        velocity(velocity) {}
  Actor(const Actor&) = delete;
  Actor& operator=(const Actor&) = delete;

 protected:
  virtual void HandleInput(const Uint8* currentKeyStates) {
    std::ignore = currentKeyStates;
  }
  virtual void Update(){};

  SDL2pp::Renderer& renderer;
  const std::shared_ptr<ResourceManager>& resource_manager;
  SDL2pp::Rect position;
  SDL2pp::Point velocity;
  SDL2pp::Optional<SDL2pp::Rect> subsprite_rect;
  SDL2pp::Texture sprite = nullptr;
  SDL2pp::Texture shadow = nullptr;
};
