/* Copyright 2016 Tony Narlock. All rights reserved. */
#ifndef SRC_SYSTEMS_PLAYER_H_
#define SRC_SYSTEMS_PLAYER_H_
#include "entityx/entityx.h"
#include "systems/collision.h"
#include "components/geometry.h"
#include "SDL2/SDL.h"
#include "input.h"
#include "resource.h"
#include <memory>
#include <vector>

// Render all Renderable entities and draw some informational text.
class PlayerSystem : public entityx::System<PlayerSystem>,
                     public entityx::Receiver<PlayerSystem> {
 public:
  explicit PlayerSystem(
      const std::unique_ptr<ResourceManager>& resource_manager,
      const std::string& sprite_key);

  virtual void update(entityx::EntityManager& entities,
                      entityx::EventManager& events,
                      entityx::TimeDelta dt) override;
  const Uint8* keys = SDL_GetKeyboardState(nullptr);
  bool spawned = false;
  double last_shot = 0;
  const unsigned int shooting_delay = 80;
  const std::unique_ptr<ResourceManager>& resource_manager;

  const std::shared_ptr<SDL2pp::Texture>& GetSprite() const {
    return resource_manager->GetTexture(sprite_key);
  }
  const std::string sprite_key;

  void configure(entityx::EventManager& events) override {
    events.subscribe<CollisionEvent>(*this);
  };
  void receive(const CollisionEvent& collision);
  std::vector<entityx::Entity> hit_bullets;
};

// Render all Renderable entities and draw some informational text.
class BulletSystem : public entityx::System<BulletSystem>,
                     public entityx::Receiver<BulletSystem> {
 public:
  explicit BulletSystem(
      const std::unique_ptr<ResourceManager>& resource_manager);

  void configure(entityx::EventManager& events) override {
    events.subscribe<entityx::Entity>(*this);
  };
  virtual void update(entityx::EntityManager& entities,
                      entityx::EventManager& events,
                      entityx::TimeDelta dt) override;
  void receive(const entityx::Entity& fire_event);
  const std::unique_ptr<ResourceManager>& resource_manager;
  std::vector<entityx::Entity> bullet_queue;
};
#endif  // SRC_SYSTEMS_PLAYER_H_
