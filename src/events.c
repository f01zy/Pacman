#include <SDL3/SDL.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

#include "defines.h"
#include "events.h"
#include "ghost.h"
#include "position.h"
#include "types.h"
#include "utility.h"

void check_level_phase(struct State *state) {
  struct Level *level = &state->game->level;
  if (level->phases.curr == 7) return;
  float now = SDL_GetTicks();
  float deltatime = (now - state->app->timers.phase_start) / 1000.0f;
  if (deltatime >= level->phases.timers[level->phases.curr]) {
    level->phases.curr++;
    state->app->timers.phase_start = now;
    set_ghosts_state(state->game, get_ghosts_state(level->phases.curr));
  }
}

void check_game_over(struct GameContext *game) {
  struct Entity *pacman = get_pacman(game);
  struct Vec2 pacman_pos = get_tile_from_pos(pacman->pos);
  for (int i = 0; i < game->entities.len; i++) {
    struct Entity *ghost = game->entities.buf[i];
    struct Vec2 ghost_pos = get_tile_from_pos(ghost->pos);
    if (ghost->type == ENTITY_GHOST && !memcmp(&pacman_pos, &ghost_pos, sizeof(pacman_pos))) game->state = GAME_STATE_PACMAN_DIE;
  }
}

void check_game_ready(struct State *state) {
  float now = SDL_GetTicks();
  float deltatime = (now - state->app->timers.program_start) / 1000.0f;
  if (deltatime > GAME_READY_TIME) {
    state->game->state = GAME_STATE_PLAYING;
    state->game->stats.is_changed = true;
    state->app->timers.game_start = now;
    state->app->timers.phase_start = now;
    state->app->timers.last_dot = now;
  }
}

void check_level_finished(struct GameContext *game) {
  if (game->level.dots.collected == game->level.dots.total) game->state = GAME_STATE_LEVEL_COMPLETE;
}

void check_ghosts_home(struct GameContext *game) {
  for (int i = 0; i < game->entities.len; i++) {
    struct Entity *entity = game->entities.buf[i];
    if (entity->type == ENTITY_GHOST && entity->as.ghost.state == GHOST_STATE_HOME && game->level.dots.collected >= entity->as.ghost.dots_to_leave_home)
      entity->as.ghost.state = GHOST_STATE_EXITING;
  }
}

void check_last_dot_timer(struct State *state) {
  float deltatime = get_deltatime(state->app->timers.last_dot);
  if (get_deltatime(state->app->timers.last_dot) >= GHOST_INACTIVITY_TIMER) {
    struct Entity *ghost = NULL;
    int dots = INT_MAX;
    for (int i = 0; i < state->game->entities.len; i++) {
      struct Entity *curr = state->game->entities.buf[i];
      if (curr->type == ENTITY_GHOST && curr->as.ghost.state == GHOST_STATE_HOME && curr->as.ghost.dots_to_leave_home < dots) {
        dots = curr->as.ghost.dots_to_leave_home;
        ghost = curr;
      }
    }
    if (ghost) {
      ghost->as.ghost.state = GHOST_STATE_EXITING;
      state->app->timers.last_dot = SDL_GetTicks();
    }
  }
}

void iterate_events(struct State *state) {
  check_game_over(state->game);
  check_level_finished(state->game);
  check_ghosts_home(state->game);
  if (state->game->state == GAME_STATE_READY) {
    check_game_ready(state);
  } else {
    check_level_phase(state);
    check_last_dot_timer(state);
  }
}
