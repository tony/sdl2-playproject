#include "stage.h"

LevelStage::LevelStage(const std::unique_ptr<SDL2pp::Renderer>& renderer,
                       const std::unique_ptr<ResourceManager>& resource_manager,
                       const std::shared_ptr<StatService>& stat_service)
    : renderer(renderer), bg_texture(resource_manager->GetTexture("bg1")) {
  game_panel =
      std::make_shared<GamePanel>(stat_service, renderer, resource_manager);
  ship = std::make_shared<Ship>(renderer, resource_manager);
  stat_service->set_ship_stats(ship->stats);
}

void LevelStage::HandleInput(const Uint8* currentKeyStates) {
  ship->HandleInput(currentKeyStates);
}

void LevelStage::Update() {
  renderer->Copy(*bg_texture, SDL2pp::NullOpt, SDL2pp::NullOpt);
  game_panel->Update();
  ship->Update();
}
