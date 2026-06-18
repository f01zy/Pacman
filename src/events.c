#include <SDL3/SDL.h>
#include <stdbool.h>
#include <string.h>

#include "events.h"
#include "ghost.h"
#include "position.h"
#include "types.h"
#include "utility.h"

void check_level_phase(struct GameContext *game) {
  struct Level *level = &game->level;
  if (level->phases.curr == 8) return;
  float now = SDL_GetTicks();
  float deltatime = (now - level->phases.start) / 1000.0f;
  if (deltatime >= game->level.phases.timers[level->phases.curr]) {
    level->phases.start = now;
    level->phases.curr++;
    if (game->state == GAME_STATE_READY) {
      game->stats.is_changed = true;
      game->state = GAME_STATE_PLAYING;
    }
    set_ghosts_state(game, level->phases.curr % 2 ? GHOST_STATE_SCATTER : GHOST_STATE_CHASE);
  }
}

void check_game_over(struct GameContext *game) {
  struct Entity *pacman = get_pacman(game);
  struct Vec2 pacman_tile_pos = get_tile_from_pos(pacman->pos);
  for (int i = 0; i < game->entities.len; i++) {
    struct Entity *ghost = game->entities.buf[i];
    if (ghost->type != ENTITY_GHOST) continue;
    struct Vec2 ghost_tile_pos = get_tile_from_pos(ghost->pos);
    if (!memcmp(&pacman_tile_pos, &ghost_tile_pos, sizeof(pacman_tile_pos))) {
      game->state = GAME_STATE_PACMAN_DIE;
      return;
    }
  }
}

void check_level_finished(struct GameContext *game) {
  if (game->level.dots.collected == game->level.dots.total) game->state = GAME_STATE_LEVEL_COMPLETE;
}

void iterate_events(struct State *state) {
  check_level_phase(state->game);
  check_game_over(state->game);
  check_level_finished(state->game);
}
