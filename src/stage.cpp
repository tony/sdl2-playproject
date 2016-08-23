/* Copyright 2016 Tony Narlock. All rights reserved. */
#include "bullet.h"
#include "stage.h"
#include "config.h"
#include "components/body.h"
#include "components/renderable.h"
#include "components/collideable.h"
#include "systems/spawn.h"
#include "systems/body.h"
#include "systems/physics.h"
#include "systems/collision.h"
#include "entityx/entityx.h"

void BodySystem::update(entityx::EntityManager& entities,
                        entityx::EventManager& events,
                        entityx::TimeDelta dt) {
  entities.each<Body>([dt](entityx::Entity entity, Body& body) {
    body.position += body.direction;
  });
}

void PhysicsSystem::update(entityx::EntityManager& entities,
                           entityx::EventManager& events,
                           entityx::TimeDelta dt) {
  entities.each<Body>([dt](entityx::Entity entity, Body& body) {});
}

// Determines if two Collideable bodies have collided. If they have it emits a
// CollisionEvent. This is used by ExplosionSystem to create explosion
// particles, but it could be used by a SoundSystem to play an explosion
// sound, etc..
//
// Uses a fairly rudimentary 2D partition system, but performs reasonably well.
CollisionSystem::CollisionSystem(
    const std::shared_ptr<SDL2pp::Renderer>& renderer)
    : size(renderer->GetLogicalSize()) {
  size.x = size.x / PARTITIONS + 1;
  size.y = size.y / PARTITIONS + 1;
}

void CollisionSystem::update(entityx::EntityManager& es,
                             entityx::EventManager& events,
                             entityx::TimeDelta dt) {
  reset();
  collect(es);
  collide(events);
};

void CollisionSystem::reset() {
  grid.clear();
  grid.resize(size.x * size.y);
}

void CollisionSystem::collect(entityx::EntityManager& entities) {
  entities.each<Body, Collideable>([this](entityx::Entity entity, Body& body,
                                          Collideable& collideable) {
    unsigned int left = static_cast<int>(body.position.x - collideable.radius) /
                        PARTITIONS,
                 top = static_cast<int>(body.position.y - collideable.radius) /
                       PARTITIONS,
                 right =
                     static_cast<int>(body.position.x + collideable.radius) /
                     PARTITIONS,
                 bottom =
                     static_cast<int>(body.position.y + collideable.radius) /
                     PARTITIONS;
    CollisionSystem::Candidate candidate{body.position, collideable.radius,
                                         entity};
    unsigned int slots[4] = {
        left + top * size.x, right + top * size.x, left + bottom * size.x,
        right + bottom * size.x,
    };
    grid[slots[0]].push_back(candidate);
    if (slots[0] != slots[1])
      grid[slots[1]].push_back(candidate);
    if (slots[1] != slots[2])
      grid[slots[2]].push_back(candidate);
    if (slots[2] != slots[3])
      grid[slots[3]].push_back(candidate);
  });
}

void CollisionSystem::collide(entityx::EventManager& events) {
  for (const std::vector<CollisionSystem::Candidate>& candidates : grid) {
    for (const CollisionSystem::Candidate& left : candidates) {
      for (const CollisionSystem::Candidate& right : candidates) {
        if (left.entity == right.entity)
          continue;
        if (collided(left, right))
          events.emit<CollisionEvent>(left.entity, right.entity);
      }
    }
  }
}

float CollisionSystem::length(const SDL2pp::Point& v) {
  return std::sqrt(v.x * v.x + v.y * v.y);
}

bool CollisionSystem::collided(const CollisionSystem::Candidate& left,
                               const CollisionSystem::Candidate& right) {
  return length(left.position - right.position) < left.radius + right.radius;
}

RenderSystem::RenderSystem(
    const std::unique_ptr<SDL2pp::Renderer>& renderer,
    const std::unique_ptr<ResourceManager>& resource_manager)
    : renderer(renderer), resource_manager(resource_manager) {}

void RenderSystem::update(entityx::EntityManager& entities,
                          entityx::EventManager& events,
                          entityx::TimeDelta dt) {
  std::ignore = events;
  std::ignore = dt;

  entities.each<Body, Renderable>(
      [this](entityx::Entity entity, Body& body, Renderable& renderable) {
        renderer->Copy(*renderable.texture,
                       SDL2pp::Rect{0, 0, renderable.texture->GetWidth(),
                                    renderable.texture->GetHeight()},
                       body.position, 0, SDL2pp::NullOpt, body.flip);
      });
}

SpawnSystem::SpawnSystem(
    const std::unique_ptr<SDL2pp::Renderer>& renderer,
    const std::unique_ptr<ResourceManager>& resource_manager)
    : renderer(renderer), resource_manager(resource_manager) {}

void SpawnSystem::update(entityx::EntityManager& entities,
                         entityx::EventManager& events,
                         entityx::TimeDelta dt) {
  std::ignore = events;
  std::ignore = dt;
  int current_ships = 0;
  entityx::ComponentHandle<Collideable> collideable;
  entities.each<Collideable>(
      [&](entityx::Entity entity, Collideable&) { ++current_ships; });
  for (int i = 0; i < max_ships - current_ships; i++) {
    entityx::Entity entity = entities.create();
    entity.assign<Collideable>(2);
    int x = MAIN_VIEWPORT_RECT.w;
    int y = RandInt() % (MAIN_VIEWPORT_RECT.h - BOTTOM_VIEWPORT_RECT.h);
    entity.assign<Body>(SDL2pp::Point{x, y}, SDL2pp::Point{-1, RandInt(-1, 1)});
    entity.assign<Renderable>(resource_manager->GetTexture("ship1_tinted"));
  }
}

LevelStage::LevelStage(const std::unique_ptr<SDL2pp::Renderer>& renderer,
                       const std::unique_ptr<ResourceManager>& resource_manager,
                       const std::shared_ptr<StatService>& stat_service,
                       const std::shared_ptr<spdlog::logger>& console)
    : renderer(renderer),
      bg_texture(resource_manager->GetTextureSheet("bg1")),
      resource_manager(resource_manager),
      console(console),
      game_panel(std::make_shared<GamePanel>(stat_service,
                                             renderer,
                                             resource_manager,
                                             console)),
      player(std::make_shared<Player>(renderer, resource_manager, console)) {
  stat_service->set_ship_stats(player->ship->stats);
  systems.add<RenderSystem>(renderer, resource_manager);
  systems.add<SpawnSystem>(renderer, resource_manager);
  systems.add<PhysicsSystem>();
  systems.add<BodySystem>();
  systems.configure();
}

void LevelStage::HandleInput(const Uint8* currentKeyStates) {
  player->HandleInput(currentKeyStates);
}

void LevelStage::SpawnEnemy() {
  auto enemy = std::make_shared<Enemy>(renderer, resource_manager, console,
                                       SDL2pp::NullOpt, SDL2pp::Point{-1, 0},
                                       SDL_FLIP_HORIZONTAL);
  enemies.push_back(enemy);
}

void LevelStage::update(entityx::TimeDelta dt) {
  Uint32 now = SDL_GetTicks();
  if (now - last_bg_scroll >= 150) {
    bg_x_scroll++;
    last_bg_scroll = now;
  }
  renderer->Copy(*bg_texture, SCREEN_RECT + SDL2pp::Point(bg_x_scroll, 0),
                 SDL2pp::NullOpt);

  game_panel->Update();
  player->ship->Update();
  systems.update<RenderSystem>(dt);
  systems.update<SpawnSystem>(dt);
  systems.update<PhysicsSystem>(dt);
  systems.update<BodySystem>(dt);

  if (now - last_enemy >= 600) {
    SpawnEnemy();
    last_enemy = now;
  }
  for (auto enemy = enemies.begin(); enemy != enemies.end();) {
    bool enemy_destroyed = false;
    (*enemy)->Update();
    for (auto bullet = player->ship->bullets.begin();
         bullet != player->ship->bullets.end();) {
      if ((*enemy)->ship->GetSubspriteRect().Intersects(
              (*bullet)->GetSubspriteRect())) {
        (*enemy)->ship->OnHitByBullet(*bullet);
        player->ship->bullets.erase(bullet);
        if ((*enemy)->ship->stats->health < 1) {
          enemy_destroyed = true;
        }
      } else {
        ++bullet;
      }
    }

    if (enemy_destroyed) {
      enemies.erase(enemy);
    } else {
      ++enemy;
    }
  }
}
