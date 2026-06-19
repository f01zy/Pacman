#ifndef GHOST_INCLUDED
#define GHOST_INCLUDED

#include "types.h"

struct Vec2 get_blinky_target_tile(struct GameContext *game, struct Entity *ghost);
struct Vec2 get_pinky_target_tile(struct GameContext *game, struct Entity *ghost);
struct Vec2 get_inky_target_tile(struct GameContext *game, struct Entity *ghost);

enum Direction get_ghost_desired_direction(struct GameContext *game, struct Entity *ghost);
void set_ghosts_state(struct GameContext *game, enum GhostState state);
void move_ghost_out_home(struct GameContext *game, struct Entity *ghost, float deltatime);

#endif
