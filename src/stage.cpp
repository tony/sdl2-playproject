#include "stage.h"

Stage::Stage(SDL2pp::Renderer& renderer, spd::logger& console)
    : renderer(renderer) {
  console.info("Stage started.");
}

LevelStage::LevelStage(SDL2pp::Renderer& renderer,
                       std::shared_ptr<ResourceManager> resource_manager,
                       const std::shared_ptr<StatService>& stat_service,
                       spd::logger& console)
    : Stage(renderer, console),
      bg_texture(resource_manager->GetTexture("bg1")) {
  game_panel =
      std::make_shared<GamePanel>(stat_service, renderer, resource_manager);
  ship = std::make_shared<Ship>(renderer, resource_manager);
  stat_service->set_ship_stats(ship->stats);
}

void LevelStage::HandleInput(const Uint8* currentKeyStates) {
  ship->HandleInput(currentKeyStates);
}

void LevelStage::Update() {
  renderer.Copy(*bg_texture, SDL2pp::NullOpt, SDL2pp::NullOpt);
  game_panel->Update();
  ship->Update();
}
