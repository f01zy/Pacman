#include "input.h"
#include "pacman.h"
#include "position.h"
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
    // TODO: поворот можно делать только тогда, когда пакман довольно близко к центру тайла
    struct Vec2 tile_pos = get_tile_from_pos(app->pacman.pos);
    struct Vec2 trans_dir = get_vec_dir(dir);
    if (app->level[tile_pos.y + trans_dir.y][tile_pos.x + trans_dir.x] == TILE_WALL) return;
    change_pacman_direction(&app->pacman, dir);
  }
}
