#pragma once

#include <SDL2pp/Rect.hh>
#include <SDL2pp/SDL2pp.hh>
#include <memory>
#include "actor.h"

class Bullet : public Actor {
 public:
  Bullet(SDL2pp::Renderer& renderer,
         SDL2pp::Rect position,
         SDL2pp::Point velocity);
  bool InBounds();
  void HandleInput(const Uint8* currentKeyStates) override final;
  void Update() override final;

 private:
  SDL2pp::Point velocity{9, 0};
};
