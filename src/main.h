bool app_init(void);
bool app_load_textures(void);
bool hero_load_textures(void);
void app_close(void);
void app_callback(SDL_Event* e, bool* quit);
void hero_callback(SDL_Event* e);
void hero_throttle_alter(SDL_Point* velocity, int x, int y);
void hero_calculate_position(SDL_Rect* position, SDL_Point* velocity);
void hero_throttle_acceleration(SDL_Point* velocity);
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
    SDL_Rect position;
    SDL_Point velocity;
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
