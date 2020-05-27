/* Copyright 2016 Tony Narlock. All rights reserved. */
#include "resource.h"

#include <SDL2pp/Surface.hh>
#include <SDL2pp/Texture.hh>
#include <memory>

ResourceManager::ResourceManager() {}

void ResourceManager::AddSurface(std::string key, std::string file_path) {
  surfaces_[key] =
      std::make_shared<SDL2pp::Surface>(SDL2pp::Surface(file_path));
}

void ResourceManager::AddSurfaceWithTransparency(std::string key,
                                                 std::string file_path,
                                                 SDL_Color alpha) {
  auto surface = std::make_shared<SDL2pp::Surface>(SDL2pp::Surface(file_path));
  surface->SetBlendMode(SDL_BLENDMODE_NONE);
  surface->SetColorKey(true, SDL_MapRGB(&surface->Lock().GetFormat(), alpha.r,
                                        alpha.g, alpha.b));
  surfaces_[key] = surface;
}

void ResourceManager::AddSurfaceWithTransparencyAndTint(std::string key,
                                                        std::string file_path,
                                                        SDL_Color alpha,
                                                        SDL_Color tint) {
  auto surface = std::make_shared<SDL2pp::Surface>(SDL2pp::Surface(file_path));
  surface->SetBlendMode(SDL_BLENDMODE_NONE);
  surface->SetColorKey(true, SDL_MapRGB(&surface->Lock().GetFormat(), alpha.r,
                                        alpha.g, alpha.b));
  surface->SetColorMod(tint.r, tint.g, tint.b);
  surface->SetAlphaMod(205);
  surfaces_[key] = surface;
}

void ResourceManager::AddTexture(std::string key, SDL2pp::Texture texture) {
  textures_[key] = std::make_shared<SDL2pp::Texture>(std::move(texture));
}

void ResourceManager::AddTexture(std::string key,
                                 std::shared_ptr<SDL2pp::Texture> texture) {
  textures_[key] = std::move(texture);
}

void ResourceManager::AddTextureSheet(std::string key,
                                      SDL2pp::Texture texture) {
  texture_sheets_[key] = std::make_shared<SDL2pp::Texture>(std::move(texture));
}

void ResourceManager::AddTextureSheet(
    std::string key,
    std::shared_ptr<SDL2pp::Texture> texture) {
  texture_sheets_[key] = std::move(texture);
}

void ResourceManager::AddFont(std::string key,
                              std::string file_path,
                              int font_size) {
  fonts_[key] = std::make_shared<SDL2pp::Font>(file_path, font_size);
}

std::map<std::string, const std::shared_ptr<SDL2pp::Texture>&>
string2texture_map(const std::map<std::string, std::string>& string_map,
                   const std::unique_ptr<ResourceManager>& resource_manager) {
  std::map<std::string, const std::shared_ptr<SDL2pp::Texture>&> texture_map;
  for (const std::pair<std::string, std::string>& i : string_map) {
    texture_map.insert({i.first, resource_manager->GetTexture(i.second)});
  }
  return texture_map;
}
