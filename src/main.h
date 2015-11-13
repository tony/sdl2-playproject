bool app_init(void);
bool app_load_media(void);
void app_close(void);
void app_callback(SDL_Event* e, bool* quit);
void hero_callback(SDL_Event* e);
void texture_render(SDL_Texture* texture, int x, int y, int w, int h);
SDL_Texture* texture_load(const char* path);

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

typedef struct Sprite {
    SDL_Texture* texture;
} Sprite;

typedef struct Hero {
    SDL_Texture* StateTextures[ KEY_PRESS_SURFACE_TOTAL ];
    Sprite* sprite;
    enum HeroState state;
} Hero;

/**
 * Namespace for global objects (window, renderer, etc.)
 */
typedef struct App {
    SDL_Window* window;
    Hero* hero;
    SDL_Renderer* renderer;
    SDL_Texture* bgTexture;
} App;
