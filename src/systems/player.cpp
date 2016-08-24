/* Copyright 2016 Tony Narlock. All rights reserved. */
#include "entityx/entityx.h"
#include "components/collideable.h"
#include "components/geometry.h"
#include "components/renderable.h"
#include "components/player.h"
#include "components/parent.h"
#include "systems/player.h"
#include "resource.h"
#include "config.h"
#include "util.h"
#include <vector>
#include <SDL2pp/SDL2pp.hh>

PlayerSystem::PlayerSystem(
    const std::unique_ptr<ResourceManager>& resource_manager,
    const std::string& sprite_key)
    : resource_manager(resource_manager), sprite_key(sprite_key) {}

void PlayerSystem::update(entityx::EntityManager& entities,
                          entityx::EventManager& events,
                          entityx::TimeDelta dt) {
  if (!spawned) {
    entityx::Entity entity = entities.create();
    entity.assign<Geometry>(SDL2pp::Point{30, 30}, SDL2pp::Point{0, 0},
                            GetSprite()->GetSize());
    entity.assign<Renderable>(GetSprite());
    entity.assign<Player>();
    spawned = true;
  }
  entities.each<Geometry, Player>([this, dt](entityx::Entity entity,
                                             Geometry& geo, Player& player) {
    if (keys[SDL_SCANCODE_UP] | keys[SDL_SCANCODE_W] | keys[SDL_SCANCODE_K]) {
      geo.position.y =
          clamp(geo.position.y - static_cast<int>(MAIN_VIEWPORT_RECT.h * 0.01),
                0, MAIN_VIEWPORT_RECT.h - GetSprite()->GetHeight());
    }

    if (keys[SDL_SCANCODE_DOWN] | keys[SDL_SCANCODE_S] | keys[SDL_SCANCODE_J]) {
      geo.position.y =
          clamp(geo.position.y + static_cast<int>(MAIN_VIEWPORT_RECT.h * 0.01),
                0, MAIN_VIEWPORT_RECT.h - GetSprite()->GetHeight());
    }

    if (keys[SDL_SCANCODE_LEFT] | keys[SDL_SCANCODE_A] | keys[SDL_SCANCODE_H]) {
      geo.position.x =
          clamp(geo.position.x - static_cast<int>(SCREEN_RECT.w * 0.01), 0,
                SCREEN_RECT.w - GetSprite()->GetWidth());
    }

    if (keys[SDL_SCANCODE_RIGHT] | keys[SDL_SCANCODE_D] |
        keys[SDL_SCANCODE_L]) {
      geo.position.x =
          clamp(geo.position.x + static_cast<int>(SCREEN_RECT.w * 0.01), 0,
                SCREEN_RECT.w - GetSprite()->GetWidth());
    }
    if (*(keys + SDL_SCANCODE_SPACE) != 0) {
      if (dt - last_shot >= shooting_delay) {
        bullet_queue.push_back({entity, geo, player});
      }
    }
  });
  for (auto bullet_event : bullet_queue) {
    events.emit<PlayerFireEvent>(bullet_event);
  }
  bullet_queue.clear();
  if (*(keys + SDL_SCANCODE_SPACE) != 0) {
    if (dt - last_shot >= shooting_delay) {
      last_shot = dt;
    }
  }
}

BulletSystem::BulletSystem(
    const std::unique_ptr<ResourceManager>& resource_manager)
    : resource_manager(resource_manager) {}

void BulletSystem::receive(const PlayerFireEvent& fire_event) {
  // Events are immutable, so we can't destroy the entities here. We defer
  // the work until the update loop.
  bullet_queue.push_back(fire_event);
}

void BulletSystem::update(entityx::EntityManager& entities,
                          entityx::EventManager& events,
                          entityx::TimeDelta dt) {
  for (auto parent : bullet_queue) {
    auto& sprite = resource_manager->GetTexture("bullet1");
    entityx::Entity entity = entities.create();
    entity.assign<Geometry>(SDL2pp::Point{parent.geometry.position.x + 30,
                                          parent.geometry.position.y + 30},
                            SDL2pp::Point{1, 0}, sprite->GetSize(), 0, 3);
    entity.assign<Renderable>(sprite);
    entity.assign<HasParent>(parent.entity);
  }
  std::cout << bullet_queue.size() << std::endl;
  bullet_queue.clear();
}
