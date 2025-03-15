/* Copyright 2016 Tony Narlock. All rights reserved. */
#ifndef SRC_RESOURCE_H_
#define SRC_RESOURCE_H_

#include <SDL2pp/Font.hh>
#include <SDL2pp/SDL2pp.hh>
#include <SDL2pp/Surface.hh>
#include <SDL2pp/Texture.hh>
#include <expected>
#include <map>
#include <memory>
#include <string>
#include <string_view>

class ResourceManager
{
public:
  ResourceManager();
  void AddSurface(std::string_view key, std::string_view file_path);
  void AddSurfaceWithTransparency(std::string_view key,
                                  std::string_view file_path,
                                  SDL_Color alpha);
  void AddSurfaceWithTransparencyAndTint(std::string_view key,
                                         std::string_view file_path,
                                         SDL_Color alpha,
                                         SDL_Color tint);
  [[nodiscard]] const std::shared_ptr<SDL2pp::Surface> &GetSurface(std::string_view key) const
  {
    return surfaces_.at(std::string(key));
  }
  void AddTexture(std::string_view key, SDL2pp::Texture texture);
  void AddTexture(std::string_view key, std::shared_ptr<SDL2pp::Texture> texture);
  [[nodiscard]] const std::shared_ptr<SDL2pp::Texture> &GetTexture(std::string_view key) const
  {
    return textures_.at(std::string(key));
  }
  [[nodiscard]] bool HasTexture(std::string_view key) const { return textures_.count(std::string(key)) > 0; }
  void AddTextureSheet(std::string_view key, SDL2pp::Texture texture);
  void AddTextureSheet(std::string_view key,
                       std::shared_ptr<SDL2pp::Texture> texture);
  [[nodiscard]] const std::shared_ptr<SDL2pp::Texture> &GetTextureSheet(
      std::string_view key) const
  {
    return texture_sheets_.at(std::string(key));
  }

  void AddFont(std::string_view key, std::string_view file_path, int font_size);
  [[nodiscard]] const std::shared_ptr<SDL2pp::Font> &GetFont(std::string_view key) const
  {
    return fonts_.at(std::string(key));
  }

  // Get texture map from string map
  [[nodiscard]] std::map<std::string, const std::shared_ptr<SDL2pp::Texture> &>
  GetTexMap(const std::map<std::string, std::string> &string_map) const
  {
    std::map<std::string, const std::shared_ptr<SDL2pp::Texture> &> result;
    for (const auto &[key, texture_name] : string_map)
    {
      const auto &texture = textures_.at(texture_name);
      result.emplace(key, texture);
    }
    return result;
  }

  // New method using std::expected for error handling
  [[nodiscard]] std::expected<std::shared_ptr<SDL2pp::Texture>, std::string>
  TryLoadTexture(std::string_view name, std::string_view path, SDL2pp::Renderer &renderer)
  {
    try
    {
      auto texture = std::make_shared<SDL2pp::Texture>(
          renderer, std::string(path));
      textures_[std::string(name)] = texture;
      return texture;
    }
    catch (const std::exception &e)
    {
      return std::unexpected(std::string("Failed to load texture: ") + e.what());
    }
  }

private:
  std::map<std::string, std::shared_ptr<SDL2pp::Font>> fonts_;
  std::map<std::string, std::shared_ptr<SDL2pp::Texture>> textures_;
  std::map<std::string, std::shared_ptr<SDL2pp::Texture>> texture_sheets_;
  std::map<std::string, std::shared_ptr<SDL2pp::Surface>> surfaces_;
};

[[nodiscard]] std::map<std::string, const std::shared_ptr<SDL2pp::Texture> &>
string2texture_map(const std::map<std::string, std::string> &string_map,
                   const std::unique_ptr<ResourceManager> &resource_manager);
#endif // SRC_RESOURCE_H_
