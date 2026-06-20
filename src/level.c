#include <SDL3/SDL.h>

#include "defines.h"
#include "entity.h"
#include "level.h"
#include "levels.h"
#include "tiles.h"
#include "types.h"

void render_level(const struct Level *level, const struct Resources *resources, SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_FRect level_rect = {
    0,
    0,
    SCALED_TILE_SIZE * LEVEL_WIDTH,
    SCALED_TILE_SIZE * LEVEL_HEIGHT,
  };
  SDL_RenderFillRect(renderer, &level_rect);
  int w = SCALED_TILE_SIZE * LEVEL_WIDTH;
  int h = SCALED_TILE_SIZE * LEVEL_HEIGHT;
  SDL_FRect dstmap = {0, 0, w, h};
  SDL_FRect srcmap = {
    0,
    0,
    TILE_SIZE * LEVEL_WIDTH,
    TILE_SIZE * LEVEL_HEIGHT,
  };
  SDL_RenderTexture(renderer, resources->tileset, &srcmap, &dstmap);
  for (int i = 0; i < LEVEL_HEIGHT; i++) {
    for (int j = 0; j < LEVEL_WIDTH; j++) {
      enum TileType type = level->buf[i][j];
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
      SDL_RenderTexture(renderer, resources->tileset, &srcrect, &dstrect);
    }
  }
}

void load_level_timers(struct Level *level) {
  level->phases.timers[7] = -1.0f;
  if (level->number == 1) {
    level->phases.timers[0] = 7.0f;
    level->phases.timers[1] = 20.0f;
    level->phases.timers[2] = 7.0f;
    level->phases.timers[3] = 20.0f;
    level->phases.timers[4] = 5.0f;
    level->phases.timers[5] = 20.0f;
    level->phases.timers[6] = 5.0f;
  } else if (level->number >= 2 && level->number <= 4) {
    level->phases.timers[0] = 7.0f;
    level->phases.timers[1] = 20.0f;
    level->phases.timers[2] = 7.0f;
    level->phases.timers[3] = 20.0f;
    level->phases.timers[4] = 5.0f;
    level->phases.timers[5] = 1033.0f;
    level->phases.timers[6] = 1.0f / FPS;
  } else {
    level->phases.timers[0] = 5.0f;
    level->phases.timers[1] = 20.0f;
    level->phases.timers[2] = 5.0f;
    level->phases.timers[3] = 20.0f;
    level->phases.timers[4] = 5.0f;
    level->phases.timers[5] = 1037.0f;
    level->phases.timers[6] = 1.0f / FPS;
  }
}

void load_level(struct State *state, bool is_update_map) {
  struct Level *level = &state->game->level;
  if (is_update_map) {
    memcpy(level->buf, default_level, sizeof(default_level));
    level->is_pacman_died = false;
  }
  free_entities(state->game);
  initialize_entities(state->game);
  load_level_timers(level);
  state->game->stats.is_changed = true;
  state->game->state = GAME_STATE_READY;
  level->dots.total = DOTS_COUNT;
  level->dots.collected = 0;
  level->phases.curr = 0;
  level->number++;
}

void iterate_level(struct State *state) { render_level(&state->game->level, &state->app->resources, state->app->renderer); }
