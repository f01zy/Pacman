#include <SDL3/SDL.h>
#include <math.h>

#include "defines.h"
#include "pacman.h"
#include "position.h"
#include "tiles.h"
#include "types.h"

void align_pacman_coordinates_x(struct Pacman *pacman) {
  struct Vec2 tile_pos = get_tile_from_pos(pacman->pos);
  pacman->pos.x = SCALED_TILE_SIZE * tile_pos.x;
}

void align_pacman_coordinates_y(struct Pacman *pacman) {
  struct Vec2 tile_pos = get_tile_from_pos(pacman->pos);
  pacman->pos.y = SCALED_TILE_SIZE * tile_pos.y;
}

void render_pacman(const struct Pacman *pacman, const struct Resources *resources, SDL_Renderer *renderer) {
  struct Tile tile;
  enum TileType tile_type;
  if (pacman->curr_dir == DIRECTION_RIGHT) {
    tile_type = TILE_PACMAN_RIGHT_1;
  } else if (pacman->curr_dir == DIRECTION_LEFT) {
    tile_type = TILE_PACMAN_LEFT_1;
  } else if (pacman->curr_dir == DIRECTION_UP) {
    tile_type = TILE_PACMAN_UP_1;
  } else if (pacman->curr_dir == DIRECTION_DOWN) {
    tile_type = TILE_PACMAN_DOWN_1;
  }
  tile_type += pacman->texture_state;
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
    pacman->pos.x - TILE_SIZE,
    pacman->pos.y - TILE_SIZE,
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

void move_pacman(struct Pacman *pacman, struct Level *level, struct Stats *stats, float deltatime) {
  struct Vec2 translated_direction = get_vec_dir(pacman->curr_dir);
  struct fVec2 new_pos = {
    pacman->pos.x + translated_direction.x * PACMAN_SPEED * deltatime,
    pacman->pos.y + translated_direction.y * PACMAN_SPEED * deltatime,
  };
  if (is_collision(level, new_pos, pacman->curr_dir)) {
    if (pacman->curr_dir == DIRECTION_UP || pacman->curr_dir == DIRECTION_DOWN) {
      align_pacman_coordinates_x(pacman);
    } else {
      align_pacman_coordinates_y(pacman);
    }
    return;
  }
  pacman->pos = new_pos;
  struct Vec2 curr_tile_pos = get_tile_from_pos(new_pos);
  enum TileType tile_type = level->buf[curr_tile_pos.y][curr_tile_pos.x];
  if (tile_type == TILE_DOT) {
    stats->score += DOT_SCORE;
    level->buf[curr_tile_pos.y][curr_tile_pos.x] = TILE_EMPTY;
  }
}

void initialize_pacman(struct Pacman *pacman) {
  pacman->pos = (struct fVec2){
    SCALED_TILE_SIZE * PACMAN_START_X,
    SCALED_TILE_SIZE * PACMAN_START_Y,
  };
  pacman->curr_dir = pacman->desired_dir = DIRECTION_LEFT;
}

void change_pacman_desired_direction(struct Pacman *pacman, enum Direction dir) { pacman->desired_dir = dir; }

void change_pacman_curr_direction(struct Pacman *pacman, const struct Level *level, enum Direction dir) {
  float tile_center = SCALED_TILE_SIZE / 2.0f;
  int pos = tile_center;
  if (dir == DIRECTION_UP || dir == DIRECTION_DOWN) {
    pos += pacman->pos.x;
  } else {
    pos += pacman->pos.y;
  }
  float pos_inside_tile = fmodf(pos, SCALED_TILE_SIZE);
  float offset = fabsf(pos_inside_tile - tile_center);
  if (offset > OFFSET_ALLOWS_CHANGE_DIR) return;
  struct Vec2 tile_pos = get_tile_from_pos(pacman->pos);
  struct Vec2 trans_dir = get_vec_dir(dir);
  struct Vec2 new_pos = {
    tile_pos.x + trans_dir.x,
    tile_pos.y + trans_dir.y,
  };
  if (new_pos.x < 0 || new_pos.x >= LEVEL_WIDTH || new_pos.y < 0 || new_pos.y >= LEVEL_HEIGHT || level->buf[new_pos.y][new_pos.x] == TILE_WALL) return;
  if (dir == DIRECTION_UP || dir == DIRECTION_DOWN) {
    align_pacman_coordinates_x(pacman);
  } else {
    align_pacman_coordinates_y(pacman);
  }
  pacman->curr_dir = dir;
}

void iterate_pacman(struct AppContext *app) {
  change_pacman_curr_direction(&app->pacman, &app->level, app->pacman.desired_dir);
  move_pacman(&app->pacman, &app->level, &app->stats, app->time.delta);
  render_pacman(&app->pacman, &app->resources, app->renderer);
}
