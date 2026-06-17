#include <SDL3/SDL.h>

#include "entity.h"
#include "input.h"
#include "types.h"
#include "utility.h"

void handle_keyboard(struct GameContext *game) {
  int numkeys;
  const bool *state = SDL_GetKeyboardState(&numkeys);
  struct Entity *pacman = get_pacman(game);
  enum Direction dir = pacman->curr_dir;
  if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]) dir = DIRECTION_RIGHT;
  if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]) dir = DIRECTION_LEFT;
  if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W]) dir = DIRECTION_UP;
  if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S]) dir = DIRECTION_DOWN;
  if (dir != pacman->curr_dir) change_entity_desired_direction(pacman, dir);
}
