#include "position.h"
#include "defines.h"
#include "types.h"

struct Vec2 get_vec_dir(enum Direction dir) {
  return (struct Vec2){dir == DIRECTION_RIGHT ? 1 : dir == DIRECTION_LEFT ? -1 : 0, dir == DIRECTION_UP ? -1 : dir == DIRECTION_DOWN ? 1 : 0};
}

struct Vec2 get_tile_from_pos(struct fVec2 pos) {
  return (struct Vec2){
    pos.x / SCALED_TILE_SIZE,
    pos.y / SCALED_TILE_SIZE,
  };
}

bool is_collision(const struct Level *level, struct fVec2 pos, enum Direction dir) {
  if (dir == DIRECTION_RIGHT || dir == DIRECTION_DOWN) {
    pos.x += SCALED_TILE_SIZE - 2;
    pos.y += SCALED_TILE_SIZE - 2;
  }
  struct Vec2 tile_pos = get_tile_from_pos(pos);
  if (level->buf[tile_pos.y][tile_pos.x] == TILE_WALL) return true;
  return false;
}
