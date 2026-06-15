#include <SDL3/SDL.h>

#include "defines.h"
#include "pacman.h"
#include "position.h"
#include "tiles.h"
#include "types.h"

void change_pacman_direction(struct Pacman *pacman, enum Direction dir) { pacman->dir = dir; }

void render_pacman(struct Pacman *pacman, SDL_Renderer *renderer, SDL_Texture *tileset) {
  struct Tile tile;
  enum TileType tile_type;
  if (pacman->dir == DIRECTION_RIGHT) {
    tile_type = TILE_PACMAN_RIGHT_1;
  } else if (pacman->dir == DIRECTION_LEFT) {
    tile_type = TILE_PACMAN_LEFT_1;
  } else if (pacman->dir == DIRECTION_UP) {
    tile_type = TILE_PACMAN_UP_1;
  } else if (pacman->dir == DIRECTION_DOWN) {
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
    pacman->pos.x,
    pacman->pos.y,
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

void move_pacman(struct Pacman *pacman, enum TileType level[LEVEL_HEIGHT][LEVEL_WIDTH], int *score, float deltatime) {
  struct Vec2 dir = get_vec_dir(pacman->dir);
  struct fVec2 pos = {
    pacman->pos.x + dir.x * PACMAN_SPEED * deltatime,
    pacman->pos.y + dir.y * PACMAN_SPEED * deltatime,
  };
  struct Vec2 tile_pos = get_tile_from_pos(pos);
  if (is_collision(level, pos)) {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Collision: (%f, %f)\n", pos.x, pos.y);
    return;
  }
  enum TileType tile_type = level[tile_pos.y][tile_pos.x];
  pacman->pos = pos;
  if (tile_type == TILE_DOT) {
    *score += DOT_SCORE;
    level[tile_pos.y][tile_pos.x] = TILE_EMPTY;
  }
}
