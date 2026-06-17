#ifndef GHOST_INCLUDED
#define GHOST_INCLUDED

#include "types.h"

struct Vec2 get_blinky_target_tile(struct GameContext *game, struct Entity *ghost);
enum Direction get_ghost_desired_direction(struct GameContext *game, struct Entity *ghost);
void run_away(struct GameContext *game);

#endif
