/* Copyright 2016 Tony Narlock. All rights reserved. */
#include "resource.h"

#include <SDL2pp/Surface.hh>
#include <SDL2pp/Texture.hh>
#include <memory>

ResourceManager::ResourceManager() {}

void ResourceManager::AddSurface(std::string_view key, std::string_view file_path)
{
  surfaces_[std::string(key)] =
      std::make_shared<SDL2pp::Surface>(SDL2pp::Surface(std::string(file_path)));
}

void ResourceManager::AddSurfaceWithTransparency(std::string_view key,
                                                 std::string_view file_path,
                                                 SDL_Color alpha)
{
  auto surface = std::make_shared<SDL2pp::Surface>(SDL2pp::Surface(std::string(file_path)));
  surface->SetBlendMode(SDL_BLENDMODE_NONE);
  surface->SetColorKey(true, SDL_MapRGB(&surface->Lock().GetFormat(), alpha.r,
                                        alpha.g, alpha.b));
  surfaces_[std::string(key)] = surface;
}

void ResourceManager::AddSurfaceWithTransparencyAndTint(std::string_view key,
                                                        std::string_view file_path,
                                                        SDL_Color alpha,
                                                        SDL_Color tint)
{
  auto surface = std::make_shared<SDL2pp::Surface>(SDL2pp::Surface(std::string(file_path)));
  surface->SetBlendMode(SDL_BLENDMODE_NONE);
  surface->SetColorKey(true, SDL_MapRGB(&surface->Lock().GetFormat(), alpha.r,
                                        alpha.g, alpha.b));
  surface->SetColorMod(tint.r, tint.g, tint.b);
  surface->SetAlphaMod(205);
  surfaces_[std::string(key)] = surface;
}

void ResourceManager::AddTexture(std::string_view key, SDL2pp::Texture texture)
{
  textures_[std::string(key)] = std::make_shared<SDL2pp::Texture>(std::move(texture));
}

void ResourceManager::AddTexture(std::string_view key,
                                 std::shared_ptr<SDL2pp::Texture> texture)
{
  textures_[std::string(key)] = texture;
}

void ResourceManager::AddTextureSheet(std::string_view key,
                                      SDL2pp::Texture texture)
{
  texture_sheets_[std::string(key)] = std::make_shared<SDL2pp::Texture>(std::move(texture));
}

void ResourceManager::AddTextureSheet(
    std::string_view key,
    std::shared_ptr<SDL2pp::Texture> texture)
{
  texture_sheets_[std::string(key)] = texture;
}

void ResourceManager::AddFont(std::string_view key,
                              std::string_view file_path,
                              int font_size)
{
  fonts_[std::string(key)] = std::make_shared<SDL2pp::Font>(std::string(file_path), font_size);
}

std::map<std::string, const std::shared_ptr<SDL2pp::Texture> &>
string2texture_map(const std::map<std::string, std::string> &string_map,
                   const std::unique_ptr<ResourceManager> &resource_manager)
{
  std::map<std::string, const std::shared_ptr<SDL2pp::Texture> &> result;
  for (const auto &[key, texture_name] : string_map)
  {
    result.emplace(key, resource_manager->GetTexture(texture_name));
  }
  return result;
}
