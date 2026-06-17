#include "pacman.h"
#include "defines.h"
#include "position.h"
#include "types.h"
#include "utility.h"

void handle_pacman_tile_interaction(struct GameContext *game) {
  struct Entity *pacman = get_pacman(game);
  struct Vec2 tile_pos = get_tile_from_pos(pacman->pos);
  enum TileType tile_type = game->level.buf[tile_pos.y][tile_pos.x];
  switch (tile_type) {
  case TILE_DOT:
  case TILE_ENERGIZER:
    if (tile_type == TILE_DOT) {
      game->stats.score += DOT_SCORE;
    } else {
      game->stats.score += ENERGIZER_SCORE;
    }
    game->stats.is_changed = true;
    game->level.buf[tile_pos.y][tile_pos.x] = TILE_EMPTY;
    break;
  default:
    break;
  }
}
