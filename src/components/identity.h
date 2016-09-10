/* Copyright 2016 Tony Narlock. All rights reserved. */
#ifndef SRC_COMPONENTS_IDENTITY_H_
#define SRC_COMPONENTS_IDENTITY_H_
#include "entityx/entityx.h"

enum class EntityIdentity { ASTEROID, BULLET, PARTICLE, SHIP, PLAYER };

struct Identity : entityx::Component<Identity> {
  Identity(EntityIdentity identity) : identity(identity) {}

  EntityIdentity identity;
};
#endif  // SRC_COMPONENTS_IDENTITY_H_
