#include "position.h"
#include "defines.h"
#include "types.h"

struct Vec2 get_vec_dir(enum Direction dir) {
  return (struct Vec2){dir == DIRECTION_RIGHT ? 1 : dir == DIRECTION_LEFT ? -1 : 0, dir == DIRECTION_UP ? -1 : dir == DIRECTION_DOWN ? 1 : 0};
}

struct Vec2 get_tile_from_pos(struct fVec2 pos) {
  return (struct Vec2){
    pos.x / (TILE_SIZE * TILE_SCALE),
    pos.y / (TILE_SIZE * TILE_SCALE),
  };
}

bool is_collision(enum TileType level[LEVEL_HEIGHT][LEVEL_WIDTH], struct fVec2 pos, enum Direction dir) {
  // TODO: пофиксить вычисление
  if (dir == DIRECTION_RIGHT || dir == DIRECTION_DOWN) {
    pos.x += TILE_SIZE * TILE_SCALE - 2;
    pos.y += TILE_SIZE * TILE_SCALE - 2;
  }
  struct Vec2 tile_pos = get_tile_from_pos(pos);
  if (level[tile_pos.y][tile_pos.x] == TILE_WALL) return true;
  return false;
}
