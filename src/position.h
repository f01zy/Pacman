#ifndef COLLISION_INCLUDED
#define COLLISION_INCLUDED

#include "types.h"

struct Vec2 get_vec_dir(enum Direction dir);
struct Vec2 get_tile_from_pos(struct fVec2 pos);
bool is_collision(const struct Level *level, struct fVec2 pos, enum Direction dir);

#endif
