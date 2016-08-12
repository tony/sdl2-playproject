#pragma once

#include <array>
#include <SDL2pp/SDL2pp.hh>
#include "resource.h"

class Actor {
 public:
  Actor(const std::unique_ptr<SDL2pp::Renderer>& renderer,
        const std::unique_ptr<ResourceManager>& resource_manager,
        SDL2pp::Point position,
        SDL2pp::Point velocity,
        SDL2pp::Rect subsprite_rect,
        const std::shared_ptr<SDL2pp::Texture>& sprite,
        const std::shared_ptr<SDL2pp::Texture>& shadow)
      : subsprite_rect(subsprite_rect),
        position(position),
        renderer(renderer),
        resource_manager(resource_manager),
        velocity(velocity),
        sprite(sprite),
        shadow(shadow) {}
  Actor(const Actor&) = delete;
  Actor& operator=(const Actor&) = delete;
  SDL2pp::Rect subsprite_rect;
  SDL2pp::Point position;

 protected:
  virtual void HandleInput(const Uint8* currentKeyStates) {
    std::ignore = currentKeyStates;
  }
  virtual void Update(){};

  const std::unique_ptr<SDL2pp::Renderer>& renderer;
  const std::unique_ptr<ResourceManager>& resource_manager;

  SDL2pp::Point velocity;
  const std::shared_ptr<SDL2pp::Texture>& sprite;
  const std::shared_ptr<SDL2pp::Texture>& shadow;

  enum class ActorState { DEFAULT, UP, DOWN, LEFT, RIGHT, TOTAL };
  ActorState state = ActorState::DEFAULT;
  std::array<SDL2pp::Rect, (int)ActorState::TOTAL> subsprites;
  SDL2pp::Rect GetSubspriteRect() {
    return subsprites[static_cast<int>(state)];
  }
};
