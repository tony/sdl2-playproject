/* Copyright 2016 Tony Narlock. All rights reserved. */
#ifndef SRC_ACTOR_H_
#define SRC_ACTOR_H_

#include <array>
#include <SDL2pp/SDL2pp.hh>
#include <SDL2pp/Optional.hh>
#include "config.h"
#include "resource.h"
#include "util.h"

class Ship;

class GraphicsComponent {
  friend Ship;

 public:
  explicit GraphicsComponent(
      const std::unique_ptr<ResourceManager>& resource_manager);
  virtual void Update(const std::shared_ptr<Ship>& actor,
                      const std::unique_ptr<SDL2pp::Renderer>& renderer) = 0;
  const std::unique_ptr<ResourceManager>& resource_manager;
};

class ShipGraphicsComponent : public GraphicsComponent {
 public:
  explicit ShipGraphicsComponent(
      const std::unique_ptr<ResourceManager>& resource_manager);

  void Update(const std::shared_ptr<Ship>& actor,
              const std::unique_ptr<SDL2pp::Renderer>& renderer) override;
};

class Actor {
  friend class Enemy;

 public:
  Actor(const std::unique_ptr<SDL2pp::Renderer>& renderer,
        const std::unique_ptr<ResourceManager>& resource_manager,
        const std::string& texture_key,
        SDL2pp::Point velocity,
        SDL2pp::Optional<SDL2pp::Point> position,
        int flip = 0)
      : renderer(renderer),
        resource_manager(resource_manager),
        texture_key(texture_key),
        velocity(velocity),
        position(position ? position.value() : GenerateSpawnPosition()),
        flip(flip) {}
  Actor(const Actor&) = delete;
  Actor& operator=(const Actor&) = delete;
  const std::shared_ptr<SDL2pp::Texture>& GetSprite() const {
    return resource_manager->GetTexture(texture_key);
  }
  SDL2pp::Rect GetSubspriteRect() const {
    return SDL2pp::Rect(GetPosition(), GetSize());
  }
  SDL2pp::Point GetSize() const { return GetSprite()->GetSize() * scale; }

  SDL2pp::Point GetPosition() const { return position; }
  bool GetHit() const { return hit; }
  int GetLastHit() const { return last_hit; }
  const std::string& GetTextureKey() const { return texture_key; }

 protected:
  static SDL2pp::Point GenerateSpawnPosition() {
    int y = RandInt() % (MAIN_VIEWPORT_RECT.h - BOTTOM_VIEWPORT_RECT.h);
    return SDL2pp::Point(MAIN_VIEWPORT_RECT.w, y);
  }

  virtual void Update() {}

  const std::unique_ptr<SDL2pp::Renderer>& renderer;
  const std::unique_ptr<ResourceManager>& resource_manager;
  const std::string texture_key;
  bool hit = false;
  Uint32 last_hit = 0;
  Uint32 last_shot = 0;
  const unsigned int shooting_delay = 80;
  SDL2pp::Point velocity;
  SDL2pp::Point position;
  unsigned int scale = 1;

  int flip = 0;

  enum class ActorState { DEFAULT, UP, DOWN, LEFT, RIGHT, TOTAL };
  ActorState state = ActorState::DEFAULT;
  std::array<SDL2pp::Rect, static_cast<int>(ActorState::TOTAL)> subsprites;
};
#endif  // SRC_ACTOR_H_
