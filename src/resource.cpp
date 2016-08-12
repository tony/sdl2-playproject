#include "resource.h"
#include <SDL2pp/Texture.hh>
#include <SDL2pp/Surface.hh>
#include <memory>

ResourceManager::ResourceManager() {}

void ResourceManager::AddSurface(std::string key, std::string file_path) {
  surfaces_[key] =
      std::make_shared<SDL2pp::Surface>(SDL2pp::Surface(file_path));
}

void ResourceManager::AddSurfaceWithTransparency(std::string key,
                                                 std::string file_path,
                                                 Uint8 alpha_r,
                                                 Uint8 alpha_g,
                                                 Uint8 alpha_b) {
  auto surface = std::make_shared<SDL2pp::Surface>(SDL2pp::Surface(file_path));
  surface->SetBlendMode(SDL_BLENDMODE_NONE);
  surface->SetColorKey(true, SDL_MapRGB(&surface->Lock().GetFormat(), alpha_r,
                                        alpha_g, alpha_b));
  surfaces_[key] = surface;
}

void ResourceManager::AddSurfaceWithTransparencyAndTint(std::string key,
                                                        std::string file_path,
                                                        Uint8 alpha_r,
                                                        Uint8 alpha_g,
                                                        Uint8 alpha_b,
                                                        Uint8 r_tint,
                                                        Uint8 g_tint,
                                                        Uint8 b_tint) {
  auto surface = std::make_shared<SDL2pp::Surface>(SDL2pp::Surface(file_path));
  surface->SetBlendMode(SDL_BLENDMODE_NONE);
  surface->SetColorKey(true, SDL_MapRGB(&surface->Lock().GetFormat(), alpha_r,
                                        alpha_g, alpha_b));
  surface->SetColorMod(r_tint, g_tint, b_tint);
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

void ResourceManager::AddFont(std::string key, SDL2pp::Font& font) {
  fonts_[key] = std::make_shared<SDL2pp::Font>(std::move(font));
}
