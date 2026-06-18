#include <SDL3/SDL.h>
#include <float.h>

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
  struct Vec2 tile_pos = get_tile_from_pos(pacman->pos);
  struct Vec2 vec_dir = get_vec_dir(pacman->curr_dir);
  struct Vec2 offset_pos = {tile_pos.x + vec_dir.x * 4, tile_pos.y + vec_dir.y * 4};
  if (pacman->curr_dir == DIRECTION_UP) offset_pos.x -= 4;
  return offset_pos;
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
  if (game->level.buf[curr_tile_pos.y][curr_tile_pos.x] == TILE_TUNEL) return ghost->curr_dir;
  switch (ghost->as.ghost.state) {
  case GHOST_STATE_CHASE:
    target = ghost->as.ghost.get_target_tile(game, ghost);
    break;
  case GHOST_STATE_SCATTER:
    target = ghost->as.ghost.scatter_target_tile;
    break;
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
    if (!check_tile_pos_validity(checked_tile_pos) || game->level.buf[checked_tile_pos.y][checked_tile_pos.x] == TILE_WALL) continue;
    distance[i] = get_distance_between_two_tiles(checked_tile_pos, target);
  }
  enum Direction ans = ghost->desired_dir;
  float ans_distance = FLT_MAX;
  for (int i = 0; i < 4; i++) {
    if (distance[i] < ans_distance || (distance[i] == ans_distance && dir_priority[i] > dir_priority[ans])) {
      ans = DIRECTION_RIGHT + i;
      ans_distance = distance[i];
    }
  }
  // TODO: они в стену ебашутся и не двигаются
  if (ans_distance == FLT_MAX) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to calculate the ghost desired direction\n");
    return ghost->desired_dir;
  }
  return ans;
}

void set_ghosts_state(struct GameContext *game, enum GhostState state) {
  for (int i = 0; i < game->entities.len; i++) {
    struct Entity *entity = game->entities.buf[i];
    if (entity->type == ENTITY_GHOST) {
      entity->as.ghost.state = state;
      entity->curr_dir = entity->desired_dir = get_opposite_direction(entity->curr_dir);
    }
  }
}
