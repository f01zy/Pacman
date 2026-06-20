#ifndef PACMAN_INCLUDED
#define PACMAN_INCLUDED

#include "types.h"

void handle_pacman_tile_interaction(struct State *state);
void handle_pacman_die(struct Entity *pacman);

#endif
