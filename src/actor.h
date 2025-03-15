/* Copyright 2016 Tony Narlock. All rights reserved. */
#ifndef SRC_ACTOR_H_
#define SRC_ACTOR_H_

// C++23 MODULE CONVERSION NOTE:
// This header could be converted to a C++23 module interface:
// export module game.actor;
// export import <SDL2pp/Optional.hh>;
// export import <SDL2pp/SDL2pp.hh>;
// export import <array>;
// export import "config.h";
// export import "resource.h";
// export import "util.h";
//
// export class Actor { ... };

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

  SDL2pp::Rect GetSubspriteRect() const
  {
    return SDL2pp::Rect(position, GetSize());
  }
  SDL2pp::Point GetSize() const
  {
    return SDL2pp::Point{
        static_cast<int>(rint(sprites.at("default")->GetSize().x) * scale),
        static_cast<int>(rint(sprites.at("default")->GetSize().y) * scale)};
  }

  SDL2pp::Point position;

  std::map<std::string, const std::shared_ptr<SDL2pp::Texture> &> sprites;

  // C++20 (supported in C++23) spaceship operator for comparing actors
  auto operator<=>(const Actor &other) const
  {
    // Compare based on position
    if (auto cmp = position.x <=> other.position.x; cmp != 0)
    {
      return cmp;
    }
    return position.y <=> other.position.y;
  }

  bool operator==(const Actor &other) const = default;

protected:
  virtual void Update(const std::unique_ptr<SDL2pp::Renderer> &renderer)
  {
    std::ignore = renderer;
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
