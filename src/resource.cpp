#include "resource.h"
#include <SDL2pp/Texture.hh>
#include <SDL2pp/Surface.hh>

ResourceManager::ResourceManager() {}

void ResourceManager::AddSurface(std::string key, std::string file_path) {
  surfaces_[key] =
      std::make_shared<SDL2pp::Surface>(SDL2pp::Surface(file_path));
}

void ResourceManager::AddSurfaceWithTransparency(std::string key,
                                                 std::string file_path,
                                                 Uint8 r,
                                                 Uint8 g,
                                                 Uint8 b) {
  auto surface = std::make_shared<SDL2pp::Surface>(SDL2pp::Surface(file_path));
  surface->SetBlendMode(SDL_BLENDMODE_NONE);
  surface->SetColorKey(true, SDL_MapRGB(&surface->Lock().GetFormat(), r, g, b));
  surfaces_[key] = surface;
}

void ResourceManager::AddSurfaceWithTransparencyAndTint(std::string key,
                                                        std::string file_path,
                                                        Uint8 r,
                                                        Uint8 g,
                                                        Uint8 b) {
  auto surface = std::make_shared<SDL2pp::Surface>(SDL2pp::Surface(file_path));
  surface->SetBlendMode(SDL_BLENDMODE_NONE);
  surface->SetColorKey(true, SDL_MapRGB(&surface->Lock().GetFormat(), r, g, b));
  surface->SetColorMod(0, 0, 0);
  surface->SetAlphaMod(205);
  surfaces_[key] = surface;
}

void ResourceManager::AddTexture(std::string key, SDL2pp::Texture texture) {
  textures_[key] = std::make_shared<SDL2pp::Texture>(std::move(texture));
}

void ResourceManager::AddFont(std::string key, SDL2pp::Font& font) {
  fonts_[key] = std::make_shared<SDL2pp::Font>(std::move(font));
}
