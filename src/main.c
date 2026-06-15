#include <stdbool.h>
#include <string.h>
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#define MAP_OFFSET_X 0
#define MAP_OFFSET_Y 0
#define MAP_WIDTH    28
#define MAP_HEIGHT   31

#define TILE_SCALE   2
#define TILE_SIZE    8
#define TITLE        "Pacman"
#define TILESET_PATH "/home/aminov/Documents/Programming/Pacman/resources/tileset.png"

enum TileType {
  TILE_EMPTY,
  TILE_WALL,
  TILE_GATE,
  TILE_DOT,
  TILE_ENERGIZER,
  TILE_FRUIT,
};

static enum TileType map[MAP_HEIGHT][MAP_WIDTH] = {
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1},
  {1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 3, 1, 1, 3, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 1},
  {1, 4, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 3, 1, 1, 3, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 4, 1},
  {1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 3, 1, 1, 3, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 1},
  {1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1},
  {1, 3, 1, 1, 1, 1, 3, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 3, 1, 1, 1, 1, 3, 1},
  {1, 3, 1, 1, 1, 1, 3, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 3, 1, 1, 1, 1, 3, 1},
  {1, 3, 3, 3, 3, 3, 3, 1, 1, 3, 3, 3, 3, 1, 1, 3, 3, 3, 3, 1, 1, 3, 3, 3, 3, 3, 3, 1},
  {1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1},
  {0, 0, 0, 0, 0, 1, 3, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 3, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 3, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 3, 1, 1, 0, 1, 1, 1, 2, 2, 1, 1, 1, 0, 1, 1, 3, 1, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 3, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 3, 1, 1, 1, 1, 1, 1},
  {0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 3, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 3, 1, 1, 1, 1, 1, 1},
  {0, 0, 0, 0, 0, 1, 3, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 3, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 3, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 3, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 3, 1, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 3, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 3, 1, 1, 1, 1, 1, 1},
  {1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1},
  {1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 3, 1, 1, 3, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 1},
  {1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 3, 1, 1, 3, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 1},
  {1, 4, 3, 3, 1, 1, 3, 3, 3, 3, 3, 3, 3, 0, 0, 3, 3, 3, 3, 3, 3, 3, 1, 1, 3, 3, 4, 1},
  {1, 1, 1, 3, 1, 1, 3, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 3, 1, 1, 3, 1, 1, 1},
  {1, 1, 1, 3, 1, 1, 3, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 3, 1, 1, 3, 1, 1, 1},
  {1, 3, 3, 3, 3, 3, 3, 1, 1, 3, 3, 3, 3, 1, 1, 3, 3, 3, 3, 1, 1, 3, 3, 3, 3, 3, 3, 1},
  {1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1},
  {1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1},
  {1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

struct Vec2 {
  int x;
  int y;
};

struct Tile {
  enum TileType type;
  struct Vec2 offset;
  struct Vec2 size;
};

static struct Tile tiles[] = {
  {TILE_DOT, {29, 1}, {1, 1}},
  {TILE_ENERGIZER, {29, 3}, {1, 1}},
};

struct AppContext {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *tileset;
  enum TileType map[MAP_HEIGHT][MAP_WIDTH];
};

void initialize_map(struct AppContext *app) { memcpy(app->map, map, sizeof(map)); }

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  SDL_SetAppMetadata("Pacman", "0.0.1", NULL);
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  int width = TILE_SIZE * MAP_WIDTH * TILE_SCALE;
  int height = TILE_SIZE * MAP_HEIGHT * TILE_SCALE;
  struct AppContext *app = (struct AppContext *)SDL_calloc(1, sizeof(struct AppContext));
  if (!SDL_CreateWindowAndRenderer(TITLE, width, height, 0, &app->window, &app->renderer)) {
    SDL_Log("Failed to create window and renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  initialize_map(app);
  SDL_Texture *tileset = IMG_LoadTexture(app->renderer, TILESET_PATH);
  SDL_SetTextureScaleMode(tileset, SDL_SCALEMODE_NEAREST);
  app->tileset = tileset;
  *appstate = app;
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  struct AppContext *app = (struct AppContext *)appstate;
  int w, h;
  SDL_GetWindowSize(app->window, &w, &h);
  SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
  SDL_RenderClear(app->renderer);

  // Render empty map
  SDL_FRect dstmap = {0, 0, w, h};
  SDL_FRect srcmap = {
    TILE_SIZE * MAP_OFFSET_X,
    TILE_SIZE * MAP_OFFSET_Y,
    TILE_SIZE * MAP_WIDTH,
    TILE_SIZE * MAP_HEIGHT,
  };
  SDL_RenderTexture(app->renderer, app->tileset, &srcmap, &dstmap);

  // Fill rendered map
  for (int i = 0; i < MAP_HEIGHT; i++) {
    for (int j = 0; j < MAP_WIDTH; j++) {
      enum TileType type = app->map[i][j];
      struct Tile tile;
      bool is_found = false;
      for (int k = 0; k < sizeof(tiles) / sizeof(tiles[0]); k++) {
        if (tiles[k].type == type) {
          is_found = true;
          tile = tiles[k];
          break;
        }
      }
      if (!is_found) continue;
      SDL_FRect dstrect = {
        TILE_SIZE * TILE_SCALE * j,
        TILE_SIZE * TILE_SCALE * i,
        TILE_SIZE * TILE_SCALE * tile.size.x,
        TILE_SIZE * TILE_SCALE * tile.size.y,
      };
      SDL_FRect srcrect = {
        TILE_SIZE * tile.offset.x,
        TILE_SIZE * tile.offset.y,
        TILE_SIZE * tile.size.x,
        TILE_SIZE * tile.size.y,
      };
      SDL_RenderTexture(app->renderer, app->tileset, &srcrect, &dstrect);
    }
  }
  SDL_RenderPresent(app->renderer);
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  if (event->type == SDL_EVENT_QUIT) return SDL_APP_SUCCESS;
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  struct AppContext *app = (struct AppContext *)appstate;
  SDL_free(app);
  SDL_DestroyTexture(app->tileset);
}
