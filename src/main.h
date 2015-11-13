bool app_init(void);
bool app_load_media(void);
void app_close(void);
void app_callback(SDL_Event* e, bool* quit);
void hero_callback(SDL_Event* e);
void texture_render(SDL_Texture* texture, int x, int y, int w, int h);
SDL_Texture* texture_load(const char* path);

enum HeroState {
    HERO_STATE_DEFAULT,
    HERO_STATE_WALK_UP,
    HERO_STATE_WALK_DOWN,
    HERO_STATE_WALK_LEFT,
    HERO_STATE_WALK_RIGHT,
    HERO_STATE_TOTAL
};

typedef struct Sprite {
    SDL_Texture* texture;
} Sprite;

typedef struct Hero {
    SDL_Texture* HeroState[ HERO_STATE_TOTAL ];
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
