#include <SDL3/SDL.h>

#include "defines.h"
#include "pacman.h"
#include "position.h"
#include "tiles.h"
#include "types.h"

void align_pacman_coordinates(struct Pacman *pacman) {
  struct Vec2 tile_pos = get_tile_from_pos(pacman->pos);
  pacman->pos.x = TILE_SIZE * TILE_SCALE * tile_pos.x;
  pacman->pos.y = TILE_SIZE * TILE_SCALE * tile_pos.y;
}

void initialize_pacman(struct Pacman *pacman) {
  pacman->pos = (struct fVec2){
    TILE_SIZE * TILE_SCALE * PACMAN_START_X,
    TILE_SIZE * TILE_SCALE * PACMAN_START_Y,
  };
  pacman->curr_dir = pacman->desired_dir = DIRECTION_LEFT;
}

void change_pacman_direction(struct Pacman *pacman, enum Direction dir) {
  align_pacman_coordinates(pacman);
  pacman->curr_dir = dir;
}

void render_pacman(struct Pacman *pacman, SDL_Renderer *renderer, SDL_Texture *tileset) {
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
    TILE_SIZE * TILE_SCALE * tile.size.x,
    TILE_SIZE * TILE_SCALE * tile.size.y,
  };
  SDL_FRect srcrect = {
    TILE_SIZE * tile.offset.x,
    TILE_SIZE * tile.offset.y,
    TILE_SIZE * tile.size.x,
    TILE_SIZE * tile.size.y,
  };
  SDL_RenderTexture(renderer, tileset, &srcrect, &dstrect);
}

void move_pacman(enum TileType level[LEVEL_HEIGHT][LEVEL_WIDTH], struct Pacman *pacman, float deltatime) {
  struct Vec2 translated_direction = get_vec_dir(pacman->curr_dir);
  struct fVec2 new_pos = {
    pacman->pos.x + translated_direction.x * PACMAN_SPEED * deltatime,
    pacman->pos.y + translated_direction.y * PACMAN_SPEED * deltatime,
  };
  if (is_collision(level, new_pos, pacman->curr_dir)) {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Collision: (%f, %f)\n", pacman->pos.x, pacman->pos.y);
    align_pacman_coordinates(pacman);
    return;
  }
  pacman->pos = new_pos;
}
