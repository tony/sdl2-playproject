#include "SDL2/SDL.h"
#include "input.h"

Input::Input() {
  keys = SDL_GetKeyboardState(nullptr);
}
