#include <math.h>

#include "defines.h"
#include "input.h"
#include "pacman.h"
#include "types.h"

void handle_keyboard(struct AppContext *app) {
  int numkeys;
  const bool *state = SDL_GetKeyboardState(&numkeys);
  enum Direction dir = app->pacman.curr_dir;
  if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]) dir = DIRECTION_RIGHT;
  if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]) dir = DIRECTION_LEFT;
  if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W]) dir = DIRECTION_UP;
  if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S]) dir = DIRECTION_DOWN;
  if (dir != app->pacman.curr_dir) {
    float tile_center = SCALED_TILE_SIZE / 2.0f;
    int pos = tile_center;
    if (dir == DIRECTION_UP || dir == DIRECTION_DOWN) {
      pos += app->pacman.pos.x;
    } else {
      pos += app->pacman.pos.y;
    }
    float pos_inside_tile = fmodf(pos, SCALED_TILE_SIZE);
    float offset = fabsf(pos_inside_tile - tile_center);
    if (offset <= OFFSET_ALLOWS_CHANGE_DIR) change_pacman_direction(app->level, &app->pacman, dir);
  }
}
