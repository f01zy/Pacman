#include <SDL3/SDL.h>
#include <stdlib.h>

#include "defines.h"
#include "entity.h"
#include "ghost.h"
#include "pacman.h"
#include "position.h"
#include "tiles.h"
#include "types.h"
#include "utility.h"

void align_entity_x(struct Entity *entity) {
  struct Vec2 tile_pos = get_tile_from_pos(entity->pos);
  entity->pos.x = (tile_pos.x * SCALED_TILE_SIZE) + (SCALED_TILE_SIZE / 2.0f);
}

void align_entity_y(struct Entity *entity) {
  struct Vec2 tile_pos = get_tile_from_pos(entity->pos);
  entity->pos.y = (tile_pos.y * SCALED_TILE_SIZE) + (SCALED_TILE_SIZE / 2.0f);
}

void render_entity(const struct Entity *entity, const struct Resources *resources, SDL_Renderer *renderer) {
  struct Tile tile;
  enum TileType tile_type = entity->texture.tiles[entity->curr_dir] + entity->texture.curr;
  bool is_found = false;
  for (int i = 0; i < sizeof(tiles) / sizeof(tiles[0]); i++) {
    if (tiles[i].type == tile_type) {
      is_found = true;
      tile = tiles[i];
      break;
    }
  }
  if (!is_found) {
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Entity tile not found\n");
    SDL_Quit();
    exit(1);
  }
  SDL_FRect dstrect = {
    entity->pos.x - (SCALED_TILE_SIZE * tile.size.x) / 2.0f,
    entity->pos.y - (SCALED_TILE_SIZE * tile.size.y) / 2.0f,
    SCALED_TILE_SIZE * tile.size.x,
    SCALED_TILE_SIZE * tile.size.y,
  };
  SDL_FRect srcrect = {
    TILE_SIZE * tile.offset.x,
    TILE_SIZE * tile.offset.y,
    TILE_SIZE * tile.size.x,
    TILE_SIZE * tile.size.y,
  };
  SDL_RenderTexture(renderer, resources->tileset, &srcrect, &dstrect);
}

void handle_tunel_teleport(struct Entity *entity) {
  float min_x = -SCALED_TILE_SIZE * 0.3f;
  float max_x = SCALED_TILE_SIZE * ((float)LEVEL_WIDTH - 0.3f);
  if (entity->pos.x < min_x) {
    entity->pos.x = max_x;
  } else if (entity->pos.x > max_x) {
    entity->pos.x = min_x;
  }
}

void move_entity(struct Entity *entity, const struct Level *level, float deltatime) {
  struct Vec2 translated_direction = get_vec_dir(entity->curr_dir);
  struct fVec2 new_pos = {
    entity->pos.x + translated_direction.x * get_entity_speed(entity, level) * deltatime,
    entity->pos.y + translated_direction.y * get_entity_speed(entity, level) * deltatime,
  };
  if (is_collision(level, new_pos, entity->curr_dir)) {
    if (entity->curr_dir == DIRECTION_RIGHT || entity->curr_dir == DIRECTION_LEFT) {
      align_entity_x(entity);
    } else {
      align_entity_y(entity);
    }
    return;
  }
  entity->pos = new_pos;
  handle_tunel_teleport(entity);
}

void change_entity_curr_direction(struct Entity *entity, const struct Level *level) {
  struct Vec2 tile_pos = get_tile_from_pos(entity->pos);
  struct Vec2 trans_dir = get_vec_dir(entity->desired_dir);
  struct Vec2 new_pos = {
    tile_pos.x + trans_dir.x,
    tile_pos.y + trans_dir.y,
  };
  if (!check_tile_pos_validity(new_pos) || level->buf[new_pos.y][new_pos.x] == TILE_WALL) return;
  if (entity->desired_dir == DIRECTION_UP || entity->desired_dir == DIRECTION_DOWN) {
    align_entity_x(entity);
  } else {
    align_entity_y(entity);
  }
  entity->curr_dir = entity->desired_dir;
}

void change_entity_animation_tile(struct Entity *entity) { entity->texture.curr = (entity->texture.curr + 1) % entity->texture.len; }

void check_entity_animation(struct Entity *entity) {
  static const float need = 1.0f / ENTITY_ANIMATION_SPEED;
  float now = SDL_GetTicks();
  float deltatime = (now - entity->delta) / 1000;
  if (deltatime >= need) {
    change_entity_animation_tile(entity);
    entity->delta = now;
  }
}

void handle_entity_center_tile(struct State *state, struct Entity *entity) {
  change_entity_curr_direction(entity, &state->game->level);
  if (entity->type == ENTITY_GHOST) {
    entity->desired_dir = get_ghost_desired_direction(state->game, entity);
  } else if (entity->type == ENTITY_PACMAN) {
    handle_pacman_tile_interaction(state->game);
  }
}

void initialize_entities(struct GameContext *game) {
  // Pacman
  enum TileType pacman_tiles[4] = {TILE_PACMAN_RIGHT_1, TILE_PACMAN_LEFT_1, TILE_PACMAN_UP_1, TILE_PACMAN_DOWN_1};
  struct Entity *pacman = create_entity(ENTITY_PACMAN, (struct fVec2){14.0f, 23.5f}, DIRECTION_LEFT, pacman_tiles, 3);

  // Blinky
  struct fVec2 blinky_pos = {14.0f, 11.5f};
  struct Vec2 blinky_scatter_target = {LEVEL_WIDTH, 0};
  enum TileType blinky_tiles[4] = {TILE_BLINKY_RIGHT_1, TILE_BLINKY_LEFT_1, TILE_BLINKY_UP_1, TILE_BLINKY_DOWN_1};
  struct Entity *blinky = create_ghost(blinky_pos, DIRECTION_RIGHT, GHOST_TYPE_BLINKY, blinky_tiles, get_blinky_target_tile, blinky_scatter_target, 0);

  // Pinky
  struct fVec2 pinky_pos = {14.0f, 14.5f};
  struct Vec2 pinky_scatter_target = {1, 1};
  enum TileType pinky_tiles[4] = {TILE_PINKY_RIGHT_1, TILE_PINKY_LEFT_1, TILE_PINKY_UP_1, TILE_PINKY_DOWN_1};
  struct Entity *pinky = create_ghost(pinky_pos, DIRECTION_UP, GHOST_TYPE_PINKY, pinky_tiles, get_pinky_target_tile, pinky_scatter_target, 0);

  // Inky
  struct fVec2 inky_pos = {16.0f, 14.5f};
  struct Vec2 inky_scatter_target = {LEVEL_WIDTH, LEVEL_HEIGHT};
  enum TileType inky_tiles[4] = {TILE_INKY_RIGHT_1, TILE_INKY_LEFT_1, TILE_INKY_UP_1, TILE_INKY_DOWN_1};
  struct Entity *inky = create_ghost(inky_pos, DIRECTION_UP, GHOST_TYPE_INKY, inky_tiles, get_inky_target_tile, inky_scatter_target, 40);

  // Buffer
  game->entities.len = 4;
  struct Entity **entities = (struct Entity **)SDL_malloc(game->entities.len * sizeof(struct Entity *));
  entities[0] = pacman;
  entities[1] = blinky;
  entities[2] = pinky;
  entities[3] = inky;
  game->entities.buf = entities;
}

void change_entity_desired_direction(struct Entity *entity, enum Direction dir) { entity->desired_dir = dir; }

void iterate_entity(struct State *state, struct Entity *entity) {
  render_entity(entity, &state->app->resources, state->app->renderer);
  check_entity_animation(entity);
  if (state->game->state != GAME_STATE_READY) {
    if (entity->type == ENTITY_GHOST && entity->as.ghost.state == GHOST_STATE_HOME) return;
    if (entity->type == ENTITY_GHOST && entity->as.ghost.state == GHOST_STATE_EXITING) {
      move_ghost_out_home(state->game, entity, state->app->time.delta);
    } else {
      move_entity(entity, &state->game->level, state->app->time.delta);
      if (is_tile_center(entity->pos, entity->curr_dir)) handle_entity_center_tile(state, entity);
    }
  }
}
