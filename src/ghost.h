#ifndef GHOST_INCLUDED
#define GHOST_INCLUDED

#include "types.h"

enum Direction get_ghost_desired_direction(struct GameContext *game, struct Entity *ghost);

#endif
