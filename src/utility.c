#include "utility.h"
#include "defines.h"
#include "position.h"
#include "types.h"

struct Entity *get_pacman(struct GameContext *game) {
  struct Entity *pacman = NULL;
  for (int i = 0; i < game->entities.len; i++) {
    struct Entity *curr = game->entities.buf[i];
    if (curr->type == ENTITY_PACMAN) {
      pacman = curr;
      break;
    }
  }
  if (pacman == NULL) {
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Pacman not found\n");
    SDL_Quit();
  }
  return pacman;
}

enum Direction get_opposite_direction(enum Direction dir) { return dir ^ 1; }

float get_entity_speed(const struct Entity *entity, const struct Level *level) {
  struct Vec2 tile_pos = get_tile_from_pos(entity->pos);
  enum TileType tile_type = level->buf[tile_pos.y][tile_pos.x];
  float scaled_speed = MAX_SPEED * TILE_SCALE;
  if (entity->type == ENTITY_PACMAN) {
    if (tile_type == TILE_DOT) {
      return scaled_speed * 0.71f;
    } else {
      return scaled_speed * 0.80f;
    }
  }
  if (entity->type == ENTITY_GHOST) {
    if (tile_type == TILE_TUNEL) {
      return scaled_speed * 0.40f;
    } else {
      return scaled_speed * 0.75f;
    }
  }
  return scaled_speed;
}

bool is_tile_center(struct fVec2 pos, enum Direction dir) {
  struct fVec2 offset = get_tile_center_offset(pos);
  if (((dir == DIRECTION_RIGHT || dir == DIRECTION_LEFT) && offset.x <= OFFSET_ALLOWS_CHANGE_DIR) ||
      ((dir == DIRECTION_UP || dir == DIRECTION_DOWN) && offset.y <= OFFSET_ALLOWS_CHANGE_DIR)) {
    return true;
  }
  return false;
}
