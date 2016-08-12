#pragma once

#include <SDL2pp/SDL2pp.hh>
#include <SDL2pp/Font.hh>
#include <SDL2pp/Texture.hh>
#include <SDL2pp/Surface.hh>
#include <map>
#include <memory>

class ResourceManager {
 public:
  ResourceManager();
  void AddSurface(std::string key, std::string file_path);
  void AddSurfaceWithTransparency(std::string key,
                                  std::string file_path,
                                  Uint8 r,
                                  Uint8 g,
                                  Uint8 b);
  void AddSurfaceWithTransparencyAndTint(std::string key,
                                         std::string file_path,
                                         Uint8 r,
                                         Uint8 g,
                                         Uint8 b);
  void AddSurfaceWithTransparencyAndTintRed(std::string key,
                                            std::string file_path,
                                            Uint8 r,
                                            Uint8 g,
                                            Uint8 b);
  const std::shared_ptr<SDL2pp::Surface>& GetSurface(std::string key) const {
    return surfaces_.at(key);
  }
  void AddTexture(std::string key, SDL2pp::Texture texture);
  void AddTexture(std::string key, std::shared_ptr<SDL2pp::Texture> texture);
  const std::shared_ptr<SDL2pp::Texture>& GetTexture(std::string key) const {
    return textures_.at(key);
  }
  void AddFont(std::string key, SDL2pp::Font& font);
  const std::shared_ptr<SDL2pp::Font>& GetFont(std::string key) const {
    return fonts_.at(key);
  }

 private:
  std::map<std::string, std::shared_ptr<SDL2pp::Font>> fonts_;
  std::map<std::string, std::shared_ptr<SDL2pp::Texture>> textures_;
  std::map<std::string, std::shared_ptr<SDL2pp::Surface>> surfaces_;
};
