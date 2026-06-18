#ifndef UTILITY_INCLUDED
#define UTILITY_INCLUDED

#include "types.h"

struct Entity *get_pacman(struct GameContext *game);
enum Direction get_opposite_direction(enum Direction dir);
float get_entity_speed(const struct Entity *entity, const struct Level *level);
bool is_tile_center(struct fVec2 pos, enum Direction dir);

#endif
