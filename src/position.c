#include "position.h"
#include "types.h"

struct Vec2 get_vec_dir(enum Direction dir) {
  return (struct Vec2){dir == DIRECTION_RIGHT ? 1 : dir == DIRECTION_LEFT ? -1 : 0, dir == DIRECTION_UP ? -1 : dir == DIRECTION_DOWN ? 1 : 0};
}

struct Vec2 get_tile_from_pos(struct fVec2 pos) {
  return (struct Vec2){
    (pos.x + TILE_SIZE) / (TILE_SIZE * TILE_SCALE),
    (pos.y + TILE_SIZE) / (TILE_SIZE * TILE_SCALE),
  };
}

bool is_collision(enum TileType level[LEVEL_HEIGHT][LEVEL_WIDTH], struct fVec2 pos) {
  struct Vec2 tile_pos = get_tile_from_pos(pos);
  int width_in_pixels = LEVEL_WIDTH * TILE_SIZE * TILE_SCALE;
  int height_in_pixels = LEVEL_HEIGHT * TILE_SIZE * TILE_SCALE;
  if (pos.x < 0 || pos.x >= width_in_pixels || pos.y < 0 || pos.y >= height_in_pixels || level[tile_pos.y][tile_pos.x] == TILE_WALL) return true;
  return false;
}
