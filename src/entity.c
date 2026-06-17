#include <SDL3/SDL.h>

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
  entity->pos.x = SCALED_TILE_SIZE * tile_pos.x;
}

void align_entity_y(struct Entity *entity) {
  struct Vec2 tile_pos = get_tile_from_pos(entity->pos);
  entity->pos.y = SCALED_TILE_SIZE * tile_pos.y;
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
  }
  SDL_FRect dstrect = {
    entity->pos.x - TILE_SIZE,
    entity->pos.y - TILE_SIZE,
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

void move_entity(struct Entity *entity, struct Level *level, float deltatime) {
  struct Vec2 translated_direction = get_vec_dir(entity->curr_dir);
  struct fVec2 new_pos = {
    entity->pos.x + translated_direction.x * entity->speed * deltatime,
    entity->pos.y + translated_direction.y * entity->speed * deltatime,
  };
  if (is_collision(level, new_pos, entity->curr_dir)) {
    if (entity->curr_dir == DIRECTION_UP || entity->curr_dir == DIRECTION_DOWN) {
      align_entity_x(entity);
    } else {
      align_entity_y(entity);
    }
    return;
  }
  entity->pos = new_pos;
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

// TODO: вынести константы
void initialize_entities(struct GameContext *game) {
  struct Entity *pacman = SDL_calloc(1, sizeof(struct Entity));
  pacman->pos = (struct fVec2){
    SCALED_TILE_SIZE * 13.0f,
    SCALED_TILE_SIZE * 17.0f,
  };
  pacman->type = ENTITY_PACMAN;
  pacman->speed = 120;
  pacman->curr_dir = pacman->desired_dir = DIRECTION_LEFT;
  pacman->texture.tiles[0] = TILE_PACMAN_RIGHT_1;
  pacman->texture.tiles[1] = TILE_PACMAN_LEFT_1;
  pacman->texture.tiles[2] = TILE_PACMAN_UP_1;
  pacman->texture.tiles[3] = TILE_PACMAN_DOWN_1;
  pacman->texture.curr = 0;
  pacman->texture.len = 3;

  struct Entity *blinky = SDL_calloc(1, sizeof(struct Entity));
  blinky->pos = (struct fVec2){
    SCALED_TILE_SIZE * 13.0f,
    SCALED_TILE_SIZE * 11.0f,
  };
  blinky->type = ENTITY_GHOST;
  blinky->as.ghost.get_target_tile = get_blinky_target_tile;
  blinky->as.ghost.scatter_target_tile = (struct Vec2){1, 1};
  blinky->speed = 100;
  blinky->curr_dir = blinky->desired_dir = DIRECTION_LEFT;
  blinky->texture.tiles[0] = TILE_BLINKY_RIGHT_1;
  blinky->texture.tiles[1] = TILE_BLINKY_LEFT_1;
  blinky->texture.tiles[2] = TILE_BLINKY_UP_1;
  blinky->texture.tiles[3] = TILE_BLINKY_DOWN_1;
  blinky->texture.curr = 0;
  blinky->texture.len = 2;

  game->entities.len = 2;
  struct Entity **entities = (struct Entity **)SDL_malloc(game->entities.len * sizeof(struct Entity *));
  entities[0] = pacman;
  entities[1] = blinky;
  game->entities.buf = entities;
}

void change_entity_desired_direction(struct Entity *entity, enum Direction dir) { entity->desired_dir = dir; }

void handle_entity_center_tile(struct State *state, struct Entity *entity) {
  change_entity_curr_direction(entity, &state->game->level);
  if (entity->type == ENTITY_GHOST) {
    entity->desired_dir = get_ghost_desired_direction(state->game, entity);
  } else if (entity->type == ENTITY_PACMAN) {
    handle_pacman_tile_interaction(state->game);
  }
}

void iterate_entity(struct State *state, struct Entity *entity) {
  move_entity(entity, &state->game->level, state->app->time.delta);
  render_entity(entity, &state->app->resources, state->app->renderer);
  check_entity_animation(entity);

  struct fVec2 offset = get_tile_center_offset(entity->pos);
  if (((entity->curr_dir == DIRECTION_RIGHT || entity->curr_dir == DIRECTION_LEFT) && offset.x <= OFFSET_ALLOWS_CHANGE_DIR) ||
      ((entity->curr_dir == DIRECTION_UP || entity->curr_dir == DIRECTION_DOWN) && offset.y <= OFFSET_ALLOWS_CHANGE_DIR)) {
    handle_entity_center_tile(state, entity);
  }
}
