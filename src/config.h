#pragma once

#include <SDL2pp/Rect.hh>

#define SHIP_MAX_BULLETS 80

const int SCREEN_WIDTH = 630;
const int SCREEN_HEIGHT = 480;
const int SHIP_SPRITE_W = 33;
const int SHIP_SPRITE_H = 33;

const SDL2pp::Rect BOTTOM_VIEWPORT_RECT = {
    0, static_cast<int>(SCREEN_HEIGHT * .9), SCREEN_WIDTH,
    static_cast<int>(SCREEN_HEIGHT * .1)};

const SDL2pp::Rect MAIN_VIEWPORT_RECT = {0, 0, SCREEN_WIDTH,
                                         static_cast<int>(SCREEN_HEIGHT * .9)};
