#include <SDL2/SDL.h>
#include <sstream>
#include "config.h"
#include "game_panel.h"
#include "util.h"

GamePanel::GamePanel(const std::shared_ptr<StatService>& stat_service,
                     const std::unique_ptr<SDL2pp::Renderer>& renderer,
                     const std::unique_ptr<ResourceManager>& resource_manager,
                     spdlog::logger& console)
    : stat_service(stat_service),
      renderer(renderer),
      resource_manager(resource_manager),
      last_message_string(""),
      console(console) {}

void GamePanel::Update() {
  DrawStats();
}

const std::shared_ptr<SDL2pp::Texture>& GamePanel::GetStatsTexture() {
  std::stringstream stats_stream;

  std::string ship_text;
  if (stat_service->ship) {
    stats_stream << "health " << stat_service->ship->health << " / "
                 << stat_service->ship->health_max;
    ship_text = stats_stream.str();
  } else {
    ship_text = "ship not loaded";
  }

  if (last_message_string.empty() ||
      last_message_string.compare(ship_text) != 0) {
    console.info("game_panel rendering new texture for message: {}", ship_text);
    last_message_string = ship_text;
    resource_manager->AddTexture(
        "game_panel_text",
        DrawText(ship_text, 5, 25, resource_manager->GetFont("terminus-18"),
                 renderer, true));
  }
  return resource_manager->GetTexture("game_panel_text");
}

void GamePanel::DrawStats() {
  renderer->SetViewport(BOTTOM_VIEWPORT_RECT);
  auto message = GetStatsTexture();
  if (last_message_string.length() > 0) {
    SDL2pp::Rect message_rect = {5, 25, message->GetWidth(),
                                 message->GetHeight()};

    renderer->Copy(*message, message_rect, message_rect);
  }
  renderer->SetViewport(MAIN_VIEWPORT_RECT);
}
