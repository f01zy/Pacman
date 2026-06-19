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

struct Entity *get_ghost(struct GameContext *game, enum GhostType type) {
  struct Entity *ghost = NULL;
  for (int i = 0; i < game->entities.len; i++) {
    struct Entity *curr = game->entities.buf[i];
    if (curr->type == ENTITY_GHOST && curr->as.ghost.type == type) {
      ghost = curr;
      break;
    }
  }
  if (ghost == NULL) {
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Ghost not found\n");
    SDL_Quit();
  }
  return ghost;
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

enum GhostState get_ghosts_state(int phase) { return phase % 2 ? GHOST_STATE_CHASE : GHOST_STATE_SCATTER; }

struct Entity *create_entity(enum EntityType type, struct fVec2 tile_pos, enum Direction dir, enum TileType base_tiles[4], int anim_len) {
  struct Entity *entity = SDL_calloc(1, sizeof(struct Entity));
  if (!entity) {
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to allocate memory for entity\n");
    return NULL;
  }
  entity->pos = (struct fVec2){tile_pos.x * SCALED_TILE_SIZE, tile_pos.y * SCALED_TILE_SIZE};
  entity->type = type;
  entity->curr_dir = entity->desired_dir = dir;
  for (int i = 0; i < 4; i++) {
    entity->texture.tiles[i] = base_tiles[i];
  }
  entity->texture.curr = 0;
  entity->texture.len = anim_len;
  return entity;
}

struct Entity *create_ghost(struct fVec2 tile_pos, enum Direction dir, enum GhostType type, enum TileType base_tiles[4], GhostTargetTile get_target,
                            struct Vec2 scatter_target, int dots_limit) {
  struct Entity *ghost = create_entity(ENTITY_GHOST, tile_pos, dir, base_tiles, 2);
  if (!ghost) return NULL;
  ghost->as.ghost.type = type;
  ghost->as.ghost.get_target_tile = get_target;
  ghost->as.ghost.scatter_target_tile = scatter_target;
  ghost->as.ghost.dots_to_leave_home = dots_limit;
  return ghost;
}
