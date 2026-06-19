#ifndef UTILITY_INCLUDED
#define UTILITY_INCLUDED

#include "types.h"

float get_entity_speed(const struct Entity *entity, const struct Level *level);
bool is_tile_center(struct fVec2 pos, enum Direction dir);
enum Direction get_opposite_direction(enum Direction dir);
enum GhostState get_ghosts_state(int phase);
struct Entity *get_pacman(struct GameContext *game);
struct Entity *get_ghost(struct GameContext *game, enum GhostType type);
struct Entity *create_entity(enum EntityType type, struct fVec2 tile_pos, enum Direction dir, enum TileType base_tiles[4], int anim_len);
struct Entity *create_ghost(struct fVec2 tile_pos, enum Direction dir, enum GhostType type, enum TileType base_tiles[4], GhostTargetTile get_target,
                            struct Vec2 scatter_target, int dots_limit);

#endif
