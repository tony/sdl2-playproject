#include "resource.h"
#include <SDL2pp/Texture.hh>
#include <SDL2pp/Surface.hh>

SDL2pp::Texture LoadImageAlpha(SDL2pp::Renderer& renderer,
                               std::string sprite_path,
                               Uint8 r,
                               Uint8 g,
                               Uint8 b) {
  auto surface = SDL2pp::Surface(sprite_path);
  surface.SetBlendMode(SDL_BLENDMODE_NONE);
  surface.SetColorKey(true, SDL_MapRGB(&surface.Lock().GetFormat(), r, g, b));
  auto image = SDL2pp::Texture(renderer, surface);
  return image;
}

SDL2pp::Texture LoadImageShadow(SDL2pp::Renderer& renderer,
                                std::string sprite_path,
                                Uint8 r,
                                Uint8 g,
                                Uint8 b) {
  auto surface = SDL2pp::Surface(sprite_path);
  surface.SetBlendMode(SDL_BLENDMODE_NONE);
  surface.SetColorKey(true, SDL_MapRGB(&surface.Lock().GetFormat(), r, g, b));
  surface.SetColorMod(0, 0, 0);
  surface.SetAlphaMod(205);
  auto image = SDL2pp::Texture(renderer, surface);
  return image;
}

ResourceManager::ResourceManager() {}

void ResourceManager::AddSurface(std::string key, std::string file_path) {
  surfaces_[key] =
      std::make_shared<SDL2pp::Surface>(SDL2pp::Surface(file_path));
}

void ResourceManager::AddTexture(std::string key, SDL2pp::Texture texture) {
  textures_[key] = std::make_shared<SDL2pp::Texture>(std::move(texture));
}

void ResourceManager::AddFont(std::string key, SDL2pp::Font& font) {
  fonts_[key] = std::make_shared<SDL2pp::Font>(std::move(font));
}
