#include <SDL3/SDL.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

#include "defines.h"
#include "events.h"
#include "ghost.h"
#include "level.h"
#include "pacman.h"
#include "position.h"
#include "types.h"
#include "utility.h"

void check_level_phase(struct State *state) {
  struct Level *level = &state->game->level;
  if (level->phases.curr == 7) return;
  if (get_deltatime(state->app->timers.phase_start) >= level->phases.timers[level->phases.curr]) {
    level->phases.curr++;
    state->app->timers.phase_start = SDL_GetTicks();
    set_ghosts_state(state->game, get_ghosts_state(level->phases.curr));
  }
}

void check_pacman_die(struct GameContext *game) {
  struct Entity *pacman = get_pacman(game);
  struct Vec2 pacman_pos = get_tile_from_pos(pacman->pos);
  for (int i = 0; i < game->entities.len; i++) {
    struct Entity *ghost = game->entities.buf[i];
    struct Vec2 ghost_pos = get_tile_from_pos(ghost->pos);
    if (ghost->type == ENTITY_GHOST && !memcmp(&pacman_pos, &ghost_pos, sizeof(pacman_pos))) {
      handle_pacman_die(pacman);
      game->state = GAME_STATE_PACMAN_DIE;
      game->level.is_pacman_died = true;
    }
  }
}

void check_pacman_die_animation_end(struct State *state) {
  struct Entity *pacman = get_pacman(state->game);
  if (state->game->state == GAME_STATE_PACMAN_DIE && pacman->texture.curr == PACMAN_DIE_TILES - 1) {
    if (--state->game->lives <= 0) {
      state->game->state = GAME_STATE_GAME_OVER;
      state->game->stats.is_changed = true;
      return;
    }
    load_level(state, false);
  }
}

void check_game_ready(struct State *state) {
  if (get_deltatime(state->app->timers.program_start) > GAME_READY_TIMER) {
    float now = SDL_GetTicks();
    state->game->state = GAME_STATE_PLAYING;
    state->game->stats.is_changed = true;
    state->app->timers.game_start = now;
    state->app->timers.phase_start = now;
    state->app->timers.last_dot = now;
  }
}

void check_level_finished(struct State *state) {
  if (state->game->level.dots.collected == state->game->level.dots.total) {
    load_level(state, true);
    state->game->level.number++;
  }
}

void check_ghosts_home(struct GameContext *game) {
  for (int i = 0; i < game->entities.len; i++) {
    struct Entity *entity = game->entities.buf[i];
    int dots_limit = get_dots_limit(game, entity);
    if (entity->type == ENTITY_GHOST && entity->as.ghost.state == GHOST_STATE_HOME && game->level.dots.collected >= dots_limit) {
      entity->as.ghost.state = GHOST_STATE_EXITING;
    }
  }
}

void check_last_dot_timer(struct State *state) {
  if (get_deltatime(state->app->timers.last_dot) >= GHOST_INACTIVITY_TIMER) {
    struct Entity *ghost = NULL;
    int dots = INT_MAX;
    for (int i = 0; i < state->game->entities.len; i++) {
      struct Entity *curr = state->game->entities.buf[i];
      int dots_limit = get_dots_limit(state->game, curr);
      if (curr->type == ENTITY_GHOST && curr->as.ghost.state == GHOST_STATE_HOME && dots_limit < dots) {
        dots = dots_limit;
        ghost = curr;
      }
    }
    if (ghost) {
      ghost->as.ghost.state = GHOST_STATE_EXITING;
      state->app->timers.last_dot = SDL_GetTicks();
    }
  }
}

void check_energizer_timer(struct State *state) {
  if (get_deltatime(state->app->timers.energizer) >= ENERGIZER_TIME) {
    state->game->state = GAME_STATE_PLAYING;
    set_ghosts_state(state->game, get_ghosts_state(state->game->level.phases.curr));
  }
}

void iterate_events(struct State *state) {
  check_ghosts_home(state->game);
  check_level_finished(state);
  if (state->game->state != GAME_STATE_PACMAN_DIE) {
    check_pacman_die(state->game);
  } else {
    check_pacman_die_animation_end(state);
  }
  if (state->game->state == GAME_STATE_READY) {
    check_game_ready(state);
  } else if (state->game->state == GAME_STATE_ENERGIZER) {
    check_energizer_timer(state);
  } else {
    check_level_phase(state);
    check_last_dot_timer(state);
  }
}
