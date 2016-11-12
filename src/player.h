#ifndef SRC_PLAYER_H_
#define SRC_PLAYER_H_

#include <memory>

class LevelStage;
class InputManager;

class Player {
 public:
  Player(const std::shared_ptr<LevelStage>& stage);

  void HandleInput(const std::shared_ptr<InputManager>& input);

  const std::shared_ptr<LevelStage>& stage;
};

#endif  // SRC_PLAYER_H_
