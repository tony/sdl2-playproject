/* Copyright 2016 Tony Narlock. All rights reserved. */
#ifndef SRC_COMPONENTS_RENDERABLE_H_
#define SRC_COMPONENTS_RENDERABLE_H_

struct Renderable {
  explicit Renderable(const std::shared_ptr<SDL2pp::Texture>& texture)
      : texture(texture) {}
  const std::shared_ptr<SDL2pp::Texture>& texture;
};
#endif  // SRC_COMPONENTS_RENDERABLE_H_
