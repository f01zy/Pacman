#include <math.h>

#include "defines.h"
#include "types.h"
#include "utility.h"

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

struct fVec2 get_tile_center_offset(struct fVec2 pos) {
  float tile_center = SCALED_TILE_SIZE / 2.0f;
  return (struct fVec2){
    fabsf(fmodf(pos.x + tile_center, SCALED_TILE_SIZE) - tile_center),
    fabsf(fmodf(pos.y + tile_center, SCALED_TILE_SIZE) - tile_center),
  };
}

enum Direction get_opposite_direction(enum Direction dir) { return dir ^ 1; }
