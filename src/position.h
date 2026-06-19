#ifndef COLLISION_INCLUDED
#define COLLISION_INCLUDED

#include "types.h"

struct Vec2 get_vec_dir(enum Direction dir);
struct Vec2 get_tile_from_pos(struct fVec2 pos);
struct Vec2 get_pacman_tile_with_offset(struct Entity *pacman, int offset);
struct fVec2 get_tile_center_offset(struct fVec2 pos);
float get_distance_between_two_tiles(struct Vec2 a, struct Vec2 b);
bool check_tile_pos_validity(struct Vec2 pos);
bool is_collision(const struct Level *level, struct fVec2 pos, enum Direction dir);

#endif
