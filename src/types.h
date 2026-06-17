#ifndef TYPES_INCLUDED
#define TYPES_INCLUDED

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "defines.h"

enum TileType {
  // Map objects
  TILE_EMPTY,
  TILE_WALL,
  TILE_GATE,

  // Collect items
  TILE_DOT,
  TILE_ENERGIZER,
  TILE_FRUIT,

  // Pacman
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

  // Blinky
  TILE_BLINKY_RIGHT_1,
  TILE_BLINKY_RIGHT_2,
  TILE_BLINKY_LEFT_1,
  TILE_BLINKY_LEFT_2,
  TILE_BLINKY_UP_1,
  TILE_BLINKY_UP_2,
  TILE_BLINKY_DOWN_1,
  TILE_BLINKY_DOWN_2,
};

enum Direction {
  DIRECTION_RIGHT,
  DIRECTION_LEFT,
  DIRECTION_UP,
  DIRECTION_DOWN,
};

enum VerticalAlign {
  VALIGN_TOP,
  VALIGN_CENTER,
  VALIGN_BOTTOM,
};

enum HorizontalAlign {
  HALIGN_LEFT,
  HALIGN_CENTER,
  HALIGN_RIGHT,
};

enum EntityType {
  ENTITY_PACMAN,
  ENTITY_GHOST,
};

enum GhostType {
  GHOST_BLINKY,
};

struct Align {
  enum HorizontalAlign h;
  enum VerticalAlign v;
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

struct Entity {
  enum EntityType type;
  struct fVec2 pos;
  struct Vec2 tile_pos;
  enum Direction curr_dir;
  enum Direction desired_dir;
  float speed;
  float delta;

  struct {
    enum TileType tiles[4];
    size_t len;
    size_t curr;
  } texture;

  union {
    struct {
      enum GhostType type;
    } ghost;
  } as;
};

struct Stats {
  int score;
  bool is_changed;
};

struct Level {
  enum TileType buf[LEVEL_HEIGHT][LEVEL_WIDTH];
  int number;
};

struct Resources {
  SDL_Texture *tileset;
  TTF_Font *font;
};

struct AppContext {
  SDL_Window *window;
  SDL_Renderer *renderer;
  struct Resources resources;

  struct {
    float prev;
    float delta;
  } time;
};

struct GameContext {
  struct Stats stats;
  struct Level level;

  struct {
    struct Entity **buf;
    size_t len;
  } entities;
};

struct State {
  struct AppContext *app;
  struct GameContext *game;
};

#endif
