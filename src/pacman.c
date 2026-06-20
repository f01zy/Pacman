#include <SDL3/SDL.h>

#include "defines.h"
#include "pacman.h"
#include "position.h"
#include "types.h"
#include "utility.h"

void handle_pacman_tile_interaction(struct State *state) {
  struct Entity *pacman = get_pacman(state->game);
  struct Vec2 tile_pos = get_tile_from_pos(pacman->pos);
  enum TileType tile_type = state->game->level.buf[tile_pos.y][tile_pos.x];
  switch (tile_type) {
  case TILE_DOT:
  case TILE_ENERGIZER:
    if (tile_type == TILE_DOT) {
      state->app->timers.last_dot = SDL_GetTicks();
      state->game->stats.score += DOT_SCORE;
      state->game->level.dots.collected++;
    } else {
      state->game->stats.score += ENERGIZER_SCORE;
    }
    state->game->stats.is_changed = true;
    state->game->level.buf[tile_pos.y][tile_pos.x] = TILE_EMPTY;
    break;
  default:
    break;
  }
}

void handle_pacman_die(struct Entity *pacman) {
  pacman->is_die = true;
  pacman->texture.len = 11;
  pacman->texture.curr = 0;
}
