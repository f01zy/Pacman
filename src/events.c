#include <SDL3/SDL.h>

#include "events.h"
#include "ghost.h"
#include "types.h"

void check_level_phase(struct GameContext *game) {
  struct Level *level = &game->level;
  if (level->curr_phase == 7) return;
  float now = SDL_GetTicks();
  float deltatime = (now - level->phase_start_time) / 1000.0f;
  if (deltatime >= game->level.phases[level->curr_phase]) {
    level->phase_start_time = now;
    level->curr_phase++;
    run_away(game);
  }
}

void iterate_events(struct State *state) { check_level_phase(state->game); }
