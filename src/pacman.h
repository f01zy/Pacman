#ifndef PACMAN_INCLUDED
#define PACMAN_INCLUDED

#include <SDL3/SDL.h>

#include "types.h"

void initialize_pacman(struct Pacman *pacman);
void change_pacman_desired_direction(struct Pacman *pacman, enum Direction dir);
void change_pacman_animation_tile(struct Pacman *pacman);
void iterate_pacman(struct AppContext *app);

#endif
