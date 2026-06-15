#include <SDL3/SDL.h>

#include "defines.h"
#include "level.h"
#include "levels.h"
#include "tiles.h"

void load_level(struct AppContext *app, int level) { memcpy(app->level, default_level, sizeof(default_level)); }

void render_level(enum TileType level[LEVEL_HEIGHT][LEVEL_WIDTH], SDL_Renderer *renderer, SDL_Texture *tileset) {
  for (int i = 0; i < LEVEL_HEIGHT; i++) {
    for (int j = 0; j < LEVEL_WIDTH; j++) {
      enum TileType type = level[i][j];
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
        SCALED_TILE_SIZE * j,
        SCALED_TILE_SIZE * i,
        SCALED_TILE_SIZE * tile.size.x,
        SCALED_TILE_SIZE * tile.size.y,
      };
      SDL_FRect srcrect = {
        TILE_SIZE * tile.offset.x,
        TILE_SIZE * tile.offset.y,
        TILE_SIZE * tile.size.x,
        TILE_SIZE * tile.size.y,
      };
      SDL_RenderTexture(renderer, tileset, &srcrect, &dstrect);
    }
  }
}
