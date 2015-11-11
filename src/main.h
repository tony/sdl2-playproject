bool initWindow();
bool loadMedia();
void appClose();
void appMainLoop(SDL_Event* e, bool* quit);
SDL_Surface* loadSurface( const char *path );

enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};
