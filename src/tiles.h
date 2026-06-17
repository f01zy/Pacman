#ifndef TILES_INCLUDED
#define TILES_INCLUDED

#include "types.h"

static const struct Tile tiles[] = {
  // Collect items
  {TILE_DOT, {29, 1}, {1, 1}},
  {TILE_ENERGIZER, {29, 3}, {1, 1}},

  // Pacman
  {TILE_PACMAN_RIGHT_1, {57, 0}, {2, 2}},
  {TILE_PACMAN_RIGHT_2, {59, 0}, {2, 2}},
  {TILE_PACMAN_RIGHT_3, {61, 0}, {2, 2}},
  {TILE_PACMAN_LEFT_1, {57, 2}, {2, 2}},
  {TILE_PACMAN_LEFT_2, {59, 2}, {2, 2}},
  {TILE_PACMAN_LEFT_3, {61, 2}, {2, 2}},
  {TILE_PACMAN_UP_1, {57, 4}, {2, 2}},
  {TILE_PACMAN_UP_2, {59, 4}, {2, 2}},
  {TILE_PACMAN_UP_3, {61, 4}, {2, 2}},
  {TILE_PACMAN_DOWN_1, {57, 6}, {2, 2}},
  {TILE_PACMAN_DOWN_2, {59, 6}, {2, 2}},
  {TILE_PACMAN_DOWN_3, {61, 6}, {2, 2}},

  // Blinky
  {TILE_BLINKY_RIGHT_1, {57, 8}, {2, 2}},
  {TILE_BLINKY_RIGHT_2, {59, 8}, {2, 2}},
  {TILE_BLINKY_LEFT_1, {61, 8}, {2, 2}},
  {TILE_BLINKY_LEFT_2, {63, 8}, {2, 2}},
  {TILE_BLINKY_UP_1, {65, 8}, {2, 2}},
  {TILE_BLINKY_UP_2, {67, 8}, {2, 2}},
  {TILE_BLINKY_DOWN_1, {69, 8}, {2, 2}},
  {TILE_BLINKY_DOWN_2, {71, 8}, {2, 2}},
};

#endif
