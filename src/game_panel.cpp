#include <SDL2/SDL.h>
#include <sstream>
#include "config.h"
#include "game_panel.h"
#include "util.h"

GamePanel::GamePanel(const std::shared_ptr<StatService>& stat_service,
                     const std::unique_ptr<SDL2pp::Renderer>& renderer,
                     const std::unique_ptr<ResourceManager>& resource_manager)
    : stat_service(stat_service),
      renderer(renderer),
      resource_manager(resource_manager) {}

void GamePanel::Update() {
  DrawStats();
}

void GamePanel::DrawStats() {
  std::stringstream ship_text;
  renderer->SetViewport(BOTTOM_VIEWPORT_RECT);

  ship_text << "health " << stat_service->ship->health << " / "
            << stat_service->ship->health_max;
  auto message =
      DrawText(ship_text.str(), 5, 25, resource_manager->GetFont("terminus-18"),
               renderer, true);
  SDL2pp::Rect message_rect = {5, 25, message.GetWidth(), message.GetHeight()};
  renderer->Copy(message, message_rect, message_rect);
  renderer->SetViewport(MAIN_VIEWPORT_RECT);
}
