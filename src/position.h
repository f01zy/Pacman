#ifndef COLLISION_INCLUDED
#define COLLISION_INCLUDED

#include "defines.h"
#include "types.h"

struct Vec2 get_vec_dir(enum Direction dir);
struct Vec2 get_tile_from_pos(struct fVec2 pos);
bool is_collision(enum TileType level[LEVEL_HEIGHT][LEVEL_WIDTH], struct fVec2 pos);

#endif
