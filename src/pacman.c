#include <SDL3/SDL.h>

#include "defines.h"
#include "ghost.h"
#include "pacman.h"
#include "position.h"
#include "types.h"
#include "utility.h"

void handle_energizer(struct State *state) {
  state->game->state = GAME_STATE_ENERGIZER;
  state->app->timers.energizer = SDL_GetTicks();
  set_ghosts_state(state->game, GHOST_STATE_FRIGHTENED);
}

void handle_pacman_tile_interaction(struct State *state) {
  struct Entity *pacman = get_pacman(state->game);
  struct Vec2 tile_pos = get_tile_from_pos(pacman->pos);
  enum TileType tile_type = state->game->level.buf[tile_pos.y][tile_pos.x];
  switch (tile_type) {
  case TILE_DOT:
  case TILE_ENERGIZER:
    if (tile_type == TILE_DOT) {
      state->game->stats.score += DOT_SCORE;
    } else {
      state->game->stats.score += ENERGIZER_SCORE;
      handle_energizer(state);
    }
    state->app->timers.last_dot = SDL_GetTicks();
    state->game->level.dots.collected++;
    state->game->stats.is_changed = true;
    state->game->level.buf[tile_pos.y][tile_pos.x] = TILE_EMPTY;
    break;
  default:
    break;
  }
}

void handle_pacman_die(struct Entity *pacman) {
  pacman->texture.len = 11;
  pacman->texture.curr = 0;
}
