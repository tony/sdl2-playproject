#ifndef HAVE_ACTOR_H_INCLUDED
#define HAVE_ACTOR_H_INCLUDED

#include <array>
#include <SDL2pp/SDL2pp.hh>
#include <SDL2pp/Optional.hh>
#include "config.h"
#include "resource.h"

class Actor {
 public:
  Actor(const std::unique_ptr<SDL2pp::Renderer>& renderer,
        const std::unique_ptr<ResourceManager>& resource_manager,
        const std::shared_ptr<SDL2pp::Texture>& sprite,
        const std::shared_ptr<SDL2pp::Texture>& shadow,
        SDL2pp::Rect subsprite_rect,
        SDL2pp::Point velocity,
        SDL2pp::Optional<SDL2pp::Point> position)
      : renderer(renderer),
        resource_manager(resource_manager),
        sprite(sprite),
        shadow(shadow),
        subsprite_rect(subsprite_rect),
        velocity(velocity),
        position(position ? position.value() : GenerateSpawnPosition()) {}
  Actor(const Actor&) = delete;
  Actor& operator=(const Actor&) = delete;
  SDL2pp::Rect GetSubspriteRect() {
    return subsprite_rect;
    // return subsprites[static_cast<int>(state)];
  }
  SDL2pp::Point GetPosition() const { return position; }

 protected:
  virtual void HandleInput(const Uint8* currentKeyStates) {
    std::ignore = currentKeyStates;
  }
  SDL2pp::Point GenerateSpawnPosition() {
    int y = arc4random() % (MAIN_VIEWPORT_RECT.h - BOTTOM_VIEWPORT_RECT.h);
    return SDL2pp::Point(MAIN_VIEWPORT_RECT.w, y);
  }

  virtual void Update(){};

  const std::unique_ptr<SDL2pp::Renderer>& renderer;
  const std::unique_ptr<ResourceManager>& resource_manager;

  const std::shared_ptr<SDL2pp::Texture>& sprite;
  const std::shared_ptr<SDL2pp::Texture>& shadow;

  SDL2pp::Rect subsprite_rect;
  SDL2pp::Point velocity;
  SDL2pp::Point position;

  enum class ActorState { DEFAULT, UP, DOWN, LEFT, RIGHT, TOTAL };
  ActorState state = ActorState::DEFAULT;
  std::array<SDL2pp::Rect, (int)ActorState::TOTAL> subsprites;
};
#endif  // HAVE_ACTOR_H_INCLUDED
