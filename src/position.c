#include <math.h>

#include "defines.h"
#include "position.h"
#include "types.h"

struct Vec2 get_pacman_tile_with_offset(struct Entity *pacman, int offset) {
  struct Vec2 tile_pos = get_tile_from_pos(pacman->pos);
  struct Vec2 vec_dir = get_vec_dir(pacman->curr_dir);
  struct Vec2 offset_pos = {
    tile_pos.x + vec_dir.x * offset,
    tile_pos.y + vec_dir.y * offset,
  };
  if (pacman->curr_dir == DIRECTION_UP) offset_pos.x -= offset;
  return offset_pos;
}

float get_distance_between_two_tiles(struct Vec2 a, struct Vec2 b) {
  float dx = a.x - b.x;
  float dy = a.y - b.y;
  return sqrt(dx * dx + dy * dy);
}

struct Vec2 get_vec_dir(enum Direction dir) {
  return (struct Vec2){dir == DIRECTION_RIGHT ? 1 : dir == DIRECTION_LEFT ? -1 : 0, dir == DIRECTION_UP ? -1 : dir == DIRECTION_DOWN ? 1 : 0};
}

bool check_tile_pos_validity(struct Vec2 pos) {
  if (pos.x < 0 || pos.x >= LEVEL_WIDTH || pos.y < 0 || pos.y >= LEVEL_HEIGHT) return false;
  return true;
}

struct Vec2 get_tile_from_pos(struct fVec2 pos) {
  return (struct Vec2){
    MIN(pos.x / SCALED_TILE_SIZE, LEVEL_WIDTH - 1),
    MIN(pos.y / SCALED_TILE_SIZE, LEVEL_HEIGHT - 1),
  };
}

struct fVec2 get_tile_center_offset(struct fVec2 pos) {
  float tile_center = SCALED_TILE_SIZE / 2.0f;
  return (struct fVec2){
    fabsf(fmodf(pos.x, SCALED_TILE_SIZE) - tile_center),
    fabsf(fmodf(pos.y, SCALED_TILE_SIZE) - tile_center),
  };
}

bool is_collision(const struct Level *level, struct fVec2 pos, enum Direction dir) {
  struct Vec2 tile_1, tile_2;
  float radius = (SCALED_TILE_SIZE / 2.0f) - 0.1f;
  switch (dir) {
  case DIRECTION_RIGHT:
    tile_1 = get_tile_from_pos((struct fVec2){pos.x + radius, pos.y - radius});
    tile_2 = get_tile_from_pos((struct fVec2){pos.x + radius, pos.y + radius});
    break;
  case DIRECTION_LEFT:
    tile_1 = get_tile_from_pos((struct fVec2){pos.x - radius, pos.y - radius});
    tile_2 = get_tile_from_pos((struct fVec2){pos.x - radius, pos.y + radius});
    break;
  case DIRECTION_DOWN:
    tile_1 = get_tile_from_pos((struct fVec2){pos.x - radius, pos.y + radius});
    tile_2 = get_tile_from_pos((struct fVec2){pos.x + radius, pos.y + radius});
    break;
  case DIRECTION_UP:
    tile_1 = get_tile_from_pos((struct fVec2){pos.x - radius, pos.y - radius});
    tile_2 = get_tile_from_pos((struct fVec2){pos.x + radius, pos.y - radius});
    break;
  }
  if (!check_tile_pos_validity(tile_1) || !check_tile_pos_validity(tile_2)) return true;
  return (level->buf[tile_1.y][tile_1.x] == TILE_WALL || level->buf[tile_2.y][tile_2.x] == TILE_WALL);
}
