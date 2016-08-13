#ifndef HAVE_CONFIG_H_INCLUDED
#define HAVE_CONFIG_H_INCLUDED

#include <SDL2pp/Rect.hh>

const int SHIP_MAX_BULLETS = 80;

const SDL2pp::Rect SCREEN_RECT = {0, 0, 630, 480};

const SDL2pp::Rect BOTTOM_VIEWPORT_RECT = {
    0, static_cast<int>(SCREEN_RECT.h * .9), SCREEN_RECT.w,
    static_cast<int>(SCREEN_RECT.h * .1)};

const SDL2pp::Rect MAIN_VIEWPORT_RECT = {0, 0, SCREEN_RECT.w,
                                         static_cast<int>(SCREEN_RECT.h * .9)};
#endif  // HAVE_CONFIG_H_INCLUDED
