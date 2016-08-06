#pragma once

#include <SDL2pp/SDL2pp.hh>

class Actor {
 public:
  Actor(SDL2pp::Renderer& renderer,
        SDL2pp::Rect position,
        SDL2pp::Point velocity,
        std::string spritePath)
      : renderer(renderer),
        position(position),
        velocity(velocity),
        sprite(SDL2pp::Texture(renderer, spritePath.c_str())) {}
  Actor(const Actor&) = delete;
  Actor& operator=(const Actor&) = delete;

 protected:
  virtual void HandleInput(const Uint8* currentKeyStates) = 0;
  virtual void Update(){};

  SDL2pp::Renderer& renderer;
  SDL2pp::Rect position;
  SDL2pp::Point velocity;
  SDL2pp::Texture sprite;
};
