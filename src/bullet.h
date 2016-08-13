#ifndef HAVE_BULLET_H_INCLUDED
#define HAVE_BULLET_H_INCLUDED

#include <SDL2pp/Rect.hh>
#include <SDL2pp/SDL2pp.hh>
#include <memory>
#include "actor.h"
#include "resource.h"

typedef struct BulletStats {
  int speed = 1;
  int size = 1;
  int rate = 1;
  int damage = 1;
  bool knockback = false;
} BulletStats;

class Bullet : public Actor {
 public:
  Bullet(const std::unique_ptr<SDL2pp::Renderer>& renderer,
         const std::unique_ptr<ResourceManager>& resource_manager,
         SDL2pp::Point position,
         SDL2pp::Point velocity);
  bool InBounds();
  void Update() override final;
  std::shared_ptr<BulletStats> stats;

 private:
  SDL2pp::Point velocity{9, 0};
};
#endif  // HAVE_BULLET_H_INCLUDED
