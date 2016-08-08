#pragma once

#include <SDL2pp/Rect.hh>
#include <SDL2pp/SDL2pp.hh>
#include <memory>
#include "actor.h"
#include "resource.h"

class Bullet : public Actor {
 public:
  Bullet(const std::unique_ptr<SDL2pp::Renderer>& renderer,
         const std::unique_ptr<ResourceManager>& resource_manager,
         SDL2pp::Rect position,
         SDL2pp::Point velocity);
  bool InBounds();
  void Update() override final;

 private:
  SDL2pp::Point velocity{9, 0};
};
