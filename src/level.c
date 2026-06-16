#include <SDL3/SDL.h>

#include "defines.h"
#include "level.h"
#include "levels.h"
#include "tiles.h"
#include "types.h"

void render_level(struct Level level, struct Resources resources, SDL_Renderer *renderer) {
  int w = SCALED_TILE_SIZE * LEVEL_WIDTH;
  int h = SCALED_TILE_SIZE * LEVEL_HEIGHT;
  SDL_FRect dstmap = {0, 0, w, h};
  SDL_FRect srcmap = {
    TILE_SIZE * LEVEL_OFFSET_X,
    TILE_SIZE * LEVEL_OFFSET_Y,
    TILE_SIZE * LEVEL_WIDTH,
    TILE_SIZE * LEVEL_HEIGHT,
  };
  SDL_RenderTexture(renderer, resources.tileset, &srcmap, &dstmap);
  for (int i = 0; i < LEVEL_HEIGHT; i++) {
    for (int j = 0; j < LEVEL_WIDTH; j++) {
      enum TileType type = level.buf[i][j];
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
      SDL_RenderTexture(renderer, resources.tileset, &srcrect, &dstrect);
    }
  }
}

void load_level(struct Level *level) { memcpy(level->buf, default_level, sizeof(default_level)); }

void iterate_level(struct AppContext *app) { render_level(app->level, app->resources, app->renderer); }
