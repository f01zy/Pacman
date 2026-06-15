#include "input.h"
#include "pacman.h"
#include "types.h"

void handle_keyboard(struct AppContext *app) {
  int numkeys;
  const bool *state = SDL_GetKeyboardState(&numkeys);

  enum Direction dir = app->pacman.dir;
  if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]) dir = DIRECTION_RIGHT;
  if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]) dir = DIRECTION_LEFT;
  if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W]) dir = DIRECTION_UP;
  if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S]) dir = DIRECTION_DOWN;
  if (dir != app->pacman.dir) {
    // TODO: чекнуть коллизии
    change_pacman_direction(&app->pacman, dir);
  }
}
