#include <SDL3/SDL.h>
#include <stdbool.h>
#include <string.h>

#include "defines.h"
#include "events.h"
#include "ghost.h"
#include "position.h"
#include "types.h"
#include "utility.h"

void check_level_phase(struct GameContext *game) {
  struct Level *level = &game->level;
  if (level->phases.curr == 7) return;
  float now = SDL_GetTicks();
  float deltatime = (now - level->phases.start) / 1000.0f;
  if (deltatime >= game->level.phases.timers[level->phases.curr]) {
    level->phases.start = now;
    level->phases.curr++;
    set_ghosts_state(game, level->phases.curr % 2 ? GHOST_STATE_CHASE : GHOST_STATE_SCATTER);
  }
}

void check_game_over(struct GameContext *game) {
  struct Entity *pacman = get_pacman(game);
  struct Vec2 pacman_tile_pos = get_tile_from_pos(pacman->pos);
  for (int i = 0; i < game->entities.len; i++) {
    struct Entity *ghost = game->entities.buf[i];
    if (ghost->type != ENTITY_GHOST) continue;
    struct Vec2 ghost_tile_pos = get_tile_from_pos(ghost->pos);
    if (!memcmp(&pacman_tile_pos, &ghost_tile_pos, sizeof(pacman_tile_pos))) game->state = GAME_STATE_PACMAN_DIE;
  }
}

void check_game_ready(struct State *state) {
  float now = SDL_GetTicks();
  float deltatime = (now - state->app->start_time) / 1000.0f;
  if (deltatime > GAME_READY_TIME) {
    state->game->state = GAME_STATE_PLAYING;
    state->game->stats.is_changed = true;
  }
}

void check_level_finished(struct GameContext *game) {
  if (game->level.dots.collected == game->level.dots.total) game->state = GAME_STATE_LEVEL_COMPLETE;
}

void iterate_events(struct State *state) {
  if (state->game->state == GAME_STATE_READY) {
    check_game_ready(state);
  } else {
    check_level_phase(state->game);
  }
  check_game_over(state->game);
  check_level_finished(state->game);
}
