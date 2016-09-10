/* Copyright 2016 Tony Narlock. All rights reserved. */
#include "components/bullet.h"
#include "components/collideable.h"
#include "components/enemy.h"
#include "components/geometry.h"
#include "components/identity.h"
#include "components/player.h"
#include "systems/collision.h"

// Determines if two Collideable bodies have collided. If they have it emits a
// CollisionEvent. This is used by ExplosionSystem to create explosion
// particles, but it could be used by a SoundSystem to play an explosion
// sound, etc..
//
// Uses a fairly rudimentary 2D partition system, but performs reasonably well.
CollisionSystem::CollisionSystem() {}

void CollisionSystem::update(entityx::EntityManager& entities,
                             entityx::EventManager& events,
                             entityx::TimeDelta dt) {
  entityx::ComponentHandle<Geometry> geo;
  entityx::ComponentHandle<Collideable> collideable;
  entityx::ComponentHandle<Identity> identity;
  entityx::ComponentHandle<Bullet> bullet;
  entityx::ComponentHandle<Enemy> enemy;
  for (entityx::Entity bul :
       entities.entities_with_components(bullet, geo, collideable)) {
    for (entityx::Entity ship :
         entities.entities_with_components(enemy, geo, collideable)) {
      if (!bul) {
        break;
      }
      auto bullet_geo = bul.component<Geometry>();
      auto ship_geo = ship.component<Geometry>();
      if (ship_geo->GetArea().Contains(bullet_geo->GetArea())) {
        // events.emit<CollisionEvent>(bul, ship);
        bul.destroy();
      }
    }
  }
};
