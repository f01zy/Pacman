#include <SDL3/SDL.h>
#include <math.h>

#include "entity.h"
#include "position.h"
#include "tiles.h"
#include "types.h"

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
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Pacman tile not found\n");
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
  float tile_center = SCALED_TILE_SIZE / 2.0f;
  int pos = tile_center;
  if (entity->desired_dir == DIRECTION_UP || entity->desired_dir == DIRECTION_DOWN) {
    pos += entity->pos.x;
  } else {
    pos += entity->pos.y;
  }
  float pos_inside_tile = fmodf(pos, SCALED_TILE_SIZE);
  float offset = fabsf(pos_inside_tile - tile_center);
  if (offset > OFFSET_ALLOWS_CHANGE_DIR) return;
  struct Vec2 tile_pos = get_tile_from_pos(entity->pos);
  struct Vec2 trans_dir = get_vec_dir(entity->desired_dir);
  struct Vec2 new_pos = {
    tile_pos.x + trans_dir.x,
    tile_pos.y + trans_dir.y,
  };
  if (level->buf[new_pos.y][new_pos.x] == TILE_WALL) return;
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

void initialize_entities(struct GameContext *game) {
  // Pacman
  struct Entity *pacman = SDL_calloc(1, sizeof(struct Entity));
  pacman->pos = (struct fVec2){
    SCALED_TILE_SIZE * PACMAN_START_X,
    SCALED_TILE_SIZE * PACMAN_START_Y,
  };
  pacman->speed = PACMAN_SPEED;
  pacman->curr_dir = pacman->desired_dir = DIRECTION_LEFT;
  pacman->texture.tiles[0] = TILE_PACMAN_RIGHT_1;
  pacman->texture.tiles[1] = TILE_PACMAN_LEFT_1;
  pacman->texture.tiles[2] = TILE_PACMAN_UP_1;
  pacman->texture.tiles[3] = TILE_PACMAN_DOWN_1;
  pacman->texture.curr = 0;
  pacman->texture.len = 3;

  // Buffer
  game->entities.len = 1;
  struct Entity **entities = (struct Entity **)SDL_malloc(game->entities.len * sizeof(struct Entity *));
  entities[0] = pacman;
  game->entities.buf = entities;
}

void change_entity_desired_direction(struct Entity *entity, enum Direction dir) { entity->desired_dir = dir; }

void iterate_entity(struct State *state, struct Entity *entity) {
  change_entity_curr_direction(entity, &state->game->level);
  move_entity(entity, &state->game->level, state->app->time.delta);
  render_entity(entity, &state->app->resources, state->app->renderer);
  check_entity_animation(entity);
}
