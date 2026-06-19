#include <SDL3/SDL.h>
#include <float.h>
#include <math.h>

#include "ghost.h"
#include "position.h"
#include "types.h"
#include "utility.h"

struct Vec2 get_blinky_target_tile(struct GameContext *game, struct Entity *ghost) {
  struct Entity *pacman = get_pacman(game);
  return get_tile_from_pos(pacman->pos);
}

struct Vec2 get_pinky_target_tile(struct GameContext *game, struct Entity *ghost) {
  struct Entity *pacman = get_pacman(game);
  return get_pacman_tile_with_offset(pacman, 4);
}

struct Vec2 get_inky_target_tile(struct GameContext *game, struct Entity *ghost) {
  struct Entity *pacman = get_pacman(game);
  struct Entity *blinky = get_ghost(game, GHOST_TYPE_BLINKY);
  struct Vec2 blinky_pos = get_tile_from_pos(blinky->pos);
  struct Vec2 offset_pos = get_pacman_tile_with_offset(pacman, 2);
  int dx = offset_pos.x - blinky_pos.x;
  int dy = offset_pos.y - blinky_pos.y;
  return (struct Vec2){
    offset_pos.x + dx,
    offset_pos.y + dy,
  };
}

enum Direction get_ghost_desired_direction(struct GameContext *game, struct Entity *ghost) {
  if (ghost->type != ENTITY_GHOST) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "You can only get direction for a ghost\n");
    return ghost->desired_dir;
  }
  struct Vec2 target;
  struct Vec2 trans_dir = get_vec_dir(ghost->curr_dir);
  struct Vec2 curr_tile_pos = get_tile_from_pos(ghost->pos);
  struct Vec2 next_tile_pos = {
    curr_tile_pos.x + trans_dir.x,
    curr_tile_pos.y + trans_dir.y,
  };

  enum TileType curr_tile_type = game->level.buf[curr_tile_pos.y][curr_tile_pos.x];
  if (curr_tile_type == TILE_TUNEL) return ghost->curr_dir;
  if (ghost->as.ghost.state == GHOST_STATE_CHASE) {
    target = ghost->as.ghost.get_target_tile(game, ghost);
  } else if (ghost->as.ghost.state == GHOST_STATE_SCATTER) {
    target = ghost->as.ghost.scatter_target_tile;
  }

  float distance[4];
  int dir_priority[4] = {0, 2, 3, 1};
  for (int i = 0; i < 4; i++) {
    distance[i] = FLT_MAX;
    enum Direction curr_dir = DIRECTION_RIGHT + i;
    if (curr_dir == get_opposite_direction(ghost->curr_dir)) continue;
    struct Vec2 curr_trans_dir = get_vec_dir(curr_dir);
    struct Vec2 checked_tile_pos = {
      next_tile_pos.x + curr_trans_dir.x,
      next_tile_pos.y + curr_trans_dir.y,
    };
    enum TileType checked_tile_type = game->level.buf[checked_tile_pos.y][checked_tile_pos.x];
    if (!check_tile_pos_validity(checked_tile_pos) || checked_tile_type == TILE_WALL || checked_tile_type == TILE_GHOST_HOUSE_DOOR) continue;
    distance[i] = get_distance_between_two_tiles(checked_tile_pos, target);
  }
  enum Direction ans = ghost->desired_dir;
  float ans_distance = FLT_MAX;
  int ans_priority = -1;
  for (int i = 0; i < 4; i++) {
    if (distance[i] == FLT_MAX) continue;
    enum Direction curr_direction = DIRECTION_RIGHT + i;
    if (distance[i] < ans_distance) {
      ans = DIRECTION_RIGHT + i;
      ans_distance = distance[i];
      ans_priority = dir_priority[i];
    } else if (distance[i] == ans_distance && dir_priority[i] > ans_priority) {
      ans = curr_direction;
      ans_priority = dir_priority[i];
    }
  }
  if (ans_distance == FLT_MAX) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to calculate the ghost desired direction\n");
    return ghost->desired_dir;
  }
  return ans;
}

void move_ghost_out_home(struct GameContext *game, struct Entity *ghost, float deltatime) {
  float speed = get_entity_speed(ghost, &game->level);
  float center_door_x = 14.0f * SCALED_TILE_SIZE;
  float outside_y = 11.5f * SCALED_TILE_SIZE;
  float EPSILON = 0.3f;

  if (fabsf(ghost->pos.x - center_door_x) > EPSILON) {
    if (ghost->pos.x < center_door_x) {
      ghost->pos.x = SDL_min(center_door_x, ghost->pos.x + speed * deltatime);
      ghost->curr_dir = DIRECTION_RIGHT;
    } else {
      ghost->pos.x = SDL_max(center_door_x, ghost->pos.x - speed * deltatime);
      ghost->curr_dir = DIRECTION_LEFT;
    }
  }

  else if (ghost->pos.y > outside_y) {
    ghost->pos.x = center_door_x;
    ghost->pos.y = SDL_max(outside_y, ghost->pos.y - speed * deltatime);
    ghost->curr_dir = DIRECTION_UP;
  }

  else {
    ghost->pos.x = center_door_x;
    ghost->pos.y = outside_y;
    ghost->curr_dir = ghost->desired_dir = DIRECTION_LEFT;
    ghost->as.ghost.state = get_ghosts_state(game->level.phases.curr);
  }
}

void set_ghosts_state(struct GameContext *game, enum GhostState state) {
  for (int i = 0; i < game->entities.len; i++) {
    struct Entity *entity = game->entities.buf[i];
    if (entity->type == ENTITY_GHOST && entity->as.ghost.state != GHOST_STATE_HOME && entity->as.ghost.state != GHOST_STATE_EXITING) {
      entity->as.ghost.state = state;
      entity->desired_dir = get_opposite_direction(entity->curr_dir);
    }
  }
}
