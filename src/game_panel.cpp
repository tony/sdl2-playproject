#include <SDL2/SDL.h>
#include <sstream>
#include "config.h"
#include "game_panel.h"
#include "util.h"

GamePanel::GamePanel(const std::shared_ptr<StatService>& stat_service,
                     SDL2pp::Renderer& renderer,
                     SDL2pp::Font& font)
    : stat_service(stat_service), renderer(renderer), font(font) {}

void GamePanel::DrawStats() {
  std::stringstream ship_text;
  renderer.SetViewport(BOTTOM_VIEWPORT_RECT);

  ship_text << "health " << stat_service->ship->health << " / "
            << stat_service->ship->health_max;
  DrawText(ship_text.str(), 5, 25, font, renderer);
  renderer.SetViewport(MAIN_VIEWPORT_RECT);
}
