bool initWindow(void);
bool appLoadMedia(void);
void appClose(void);
void appMainLoop(SDL_Event* e, bool* quit);
void heroLoop(SDL_Event* e);
void renderTexture(SDL_Texture* texture, int x, int y, int w, int h);
SDL_Texture* loadTexture(const char* path);

enum StateTextures {
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

enum HeroState {
    Walk,
    Run,
    Attack,
    Stand
};

typedef struct Hero {
    SDL_Texture* StateTextures[ KEY_PRESS_SURFACE_TOTAL ];
    SDL_Texture* texture;
    enum HeroState state;
} Hero;


/**
 * Namespace for global objects (window, renderer, etc.)
 */
typedef struct App {
    SDL_Window* window;
    Hero* hero;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
} App;
