/* Copyright 2016 Tony Narlock. All rights reserved. */
#ifndef SRC_ACTOR_H_
#define SRC_ACTOR_H_

#include <SDL2pp/Optional.hh>
#include <SDL2pp/SDL2pp.hh>
#include <array>
#include "config.h"
#include "resource.h"
#include "util.h"

class Actor {
 public:
  Actor(std::map<std::string, const std::shared_ptr<SDL2pp::Texture>&> sprites,
        SDL2pp::Point position,
        SDL2pp::Point velocity,
        int flip = 0,
        float scale = 1)
      : position(position),
        sprites(sprites),
        velocity(velocity),
        flip(flip),
        scale(scale) {}
  Actor(const Actor&) = delete;
  Actor& operator=(const Actor&) = delete;

  SDL2pp::Rect GetSubspriteRect() const {
    return SDL2pp::Rect(position, GetSize());
  }
  SDL2pp::Point GetSize() const {
    return SDL2pp::Point{
        static_cast<int>(rint(sprites.at("default")->GetSize().x) * scale),
        static_cast<int>(rint(sprites.at("default")->GetSize().y) * scale)};
  }

  SDL2pp::Point position;

  std::map<std::string, const std::shared_ptr<SDL2pp::Texture>&> sprites;

 protected:
  virtual void Update(const std::unique_ptr<SDL2pp::Renderer>& renderer) {
    std::ignore = renderer;
  }

  SDL2pp::Point velocity;
  int flip;
  float scale;

  bool hit = false;
  Uint32 last_hit = 0;
  Uint32 last_shot = 0;
  const unsigned int shooting_delay = 80;

  enum class ActorState { DEFAULT, UP, DOWN, LEFT, RIGHT, TOTAL };
  ActorState state = ActorState::DEFAULT;
  std::array<SDL2pp::Rect, static_cast<int>(ActorState::TOTAL)> subsprites;
};
#endif  // SRC_ACTOR_H_
