bool initWindow();
bool appLoadMedia();
void appClose();
void appMainLoop(SDL_Event* e, bool* quit);
SDL_Surface* loadSurface(const char* path);
SDL_Texture* loadTexture(const char* path);

enum KeyPressTextures {
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

struct App {
    SDL_Window* window;
    SDL_Surface* screenSurface;
    SDL_Texture* keyPressTextures[ KEY_PRESS_SURFACE_TOTAL ];
    SDL_Texture* currentTexture;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
};

typedef struct App App;
