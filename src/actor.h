/* Copyright 2016 Tony Narlock. All rights reserved. */
#ifndef SRC_ACTOR_H_
#define SRC_ACTOR_H_

// We'll keep the header guard for backward compatibility
// The C++23 module declaration would look like this:
// export module game.actor;
// export import <SDL2pp/Optional.hh>;
// export import <SDL2pp/SDL2pp.hh>;
// export import <array>;
// export import "config.h";
// export import "resource.h";
// export import "util.h";

#include <SDL2pp/Optional.hh>
#include <SDL2pp/SDL2pp.hh>
#include <array>

#include "config.h"
#include "resource.h"
#include "util.h"

class Actor
{
public:
  Actor(std::map<std::string, const std::shared_ptr<SDL2pp::Texture> &> sprites,
        SDL2pp::Point position,
        SDL2pp::Point velocity,
        int flip = 0,
        float scale = 1)
      : position(position),
        sprites(sprites),
        velocity(velocity),
        flip(flip),
        scale(scale) {}
  Actor(const Actor &) = delete;
  Actor &operator=(const Actor &) = delete;
  virtual ~Actor() = default;

  [[nodiscard]] SDL2pp::Rect GetSubspriteRect() const
  {
    return SDL2pp::Rect(position, GetSize());
  }

  [[nodiscard]] SDL2pp::Point GetSize() const
  {
    return SDL2pp::Point{
        static_cast<int>(rint(sprites.at("default")->GetSize().x) * scale),
        static_cast<int>(rint(sprites.at("default")->GetSize().y) * scale)};
  }

  SDL2pp::Point position;

  std::map<std::string, const std::shared_ptr<SDL2pp::Texture> &> sprites;

  // C++20 (supported in C++23) spaceship operator for comparing actors
  auto operator<=>(const Actor &other) const = default;

protected:
  virtual void Update(const std::unique_ptr<SDL2pp::Renderer> &renderer)
  {
    std::ignore = renderer;

    // Example of C++23 feature:
    // if this was a C++23 module, we could use std::print for debug output
    // std::print("Actor at position ({}, {})\n", position.x, position.y);
  }

  SDL2pp::Point velocity;
  int flip;
  float scale;

  bool hit = false;
  Uint32 last_hit = 0;
  Uint32 last_shot = 0;
  const unsigned int shooting_delay = 80;

  enum class ActorState
  {
    DEFAULT,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    TOTAL
  };
  ActorState state = ActorState::DEFAULT;
  std::array<SDL2pp::Rect, static_cast<int>(ActorState::TOTAL)> subsprites;
};
#endif // SRC_ACTOR_H_
