#ifndef TYPES_INCLUDED
#define TYPES_INCLUDED

#include "defines.h"
#include <SDL3/SDL.h>

enum TileType {
  // Map objects
  TILE_EMPTY,
  TILE_WALL,
  TILE_GATE,

  // Collect items
  TILE_DOT,
  TILE_ENERGIZER,
  TILE_FRUIT,

  // Entities
  TILE_PACMAN_RIGHT_1,
  TILE_PACMAN_RIGHT_2,
  TILE_PACMAN_RIGHT_3,
  TILE_PACMAN_LEFT_1,
  TILE_PACMAN_LEFT_2,
  TILE_PACMAN_LEFT_3,
  TILE_PACMAN_UP_1,
  TILE_PACMAN_UP_2,
  TILE_PACMAN_UP_3,
  TILE_PACMAN_DOWN_1,
  TILE_PACMAN_DOWN_2,
  TILE_PACMAN_DOWN_3,
};

enum Direction {
  DIRECTION_RIGHT,
  DIRECTION_LEFT,
  DIRECTION_UP,
  DIRECTION_DOWN,
};

struct Vec2 {
  int x;
  int y;
};

struct fVec2 {
  float x;
  float y;
};

struct Tile {
  enum TileType type;
  struct Vec2 offset;
  struct Vec2 size;
};

struct Pacman {
  struct fVec2 pos;
  enum Direction dir;
  int texture_state;
};

struct AppContext {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *tileset;
  struct Pacman pacman;
  enum TileType level[LEVEL_HEIGHT][LEVEL_WIDTH];
  int level_number;
  int score;
  float prev_frame_time;
};

#endif
