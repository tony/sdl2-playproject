#include "bullet.h"
#include "stage.h"
#include "config.h"

LevelStage::LevelStage(const std::unique_ptr<SDL2pp::Renderer>& renderer,
                       const std::unique_ptr<ResourceManager>& resource_manager,
                       const std::shared_ptr<StatService>& stat_service,
                       const std::shared_ptr<spdlog::logger>& console)
    : renderer(renderer),
      bg_texture(resource_manager->GetTexture("bg1")),
      resource_manager(resource_manager),
      console(console) {
  game_panel = std::make_shared<GamePanel>(stat_service, renderer,
                                           resource_manager, console);
  ship = std::make_shared<Ship>(renderer, resource_manager);
  stat_service->set_ship_stats(ship->stats);
}

void LevelStage::HandleInput(const Uint8* currentKeyStates) {
  ship->HandleInput(currentKeyStates);
}

void LevelStage::SpawnEnemy() {
  int y_position = 0 + (arc4random() % (SCREEN_RECT.h - 0 + 1));
  auto position = SDL2pp::Point(SCREEN_RECT.w, y_position);

  enemies.push_back(std::make_shared<Enemy>(renderer, resource_manager, console,
                                            position, SDL2pp::Point{-1, 0}));
}

void LevelStage::Update() {
  renderer->Copy(*bg_texture, SDL2pp::NullOpt, SDL2pp::NullOpt);
  game_panel->Update();
  ship->Update();
  Uint32 now = SDL_GetTicks();
  if (now - last_enemy >= 600) {
    SpawnEnemy();
    last_enemy = now;
  }
  for (auto& enemy : enemies) {
    enemy->Update();
    for (auto& bullet : ship->bullets) {
      if (SDL2pp::Rect(enemy->position, enemy->subsprite_rect.GetSize())
              .Contains(bullet->position)) {
        enemy->Strike(bullet);
      }
    }
  }
}
