#ifndef UTILITY_INCLUDED
#define UTILITY_INCLUDED

#include "types.h"

struct Entity *get_pacman(struct GameContext *game);
enum Direction get_opposite_direction(enum Direction dir);
float get_entity_speed(const struct Entity *entity, const struct Level *level);

#endif
