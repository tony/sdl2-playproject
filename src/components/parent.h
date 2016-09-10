/* Copyright 2016 Tony Narlock. All rights reserved. */
#ifndef SRC_COMPONENTS_PARENT_H_
#define SRC_COMPONENTS_PARENT_H_
#include "entityx/entityx.h"

struct HasParent {
  explicit HasParent(entityx::Entity parent) : parent(parent) {}
  entityx::Entity parent;
};
#endif  // SRC_COMPONENTS_PARENT_H_
