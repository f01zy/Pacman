#ifndef TYPES_INCLUDED
#define TYPES_INCLUDED

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "defines.h"

// ===================== ENUMS =====================

enum TileType {
  TILE_EMPTY,
  TILE_WALL,
  TILE_TUNEL,
  TILE_DOT,
  TILE_ENERGIZER,
  TILE_GHOST_HOUSE_DOOR,
  TILE_FRUIT,
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
  TILE_BLINKY_RIGHT_1,
  TILE_BLINKY_RIGHT_2,
  TILE_BLINKY_LEFT_1,
  TILE_BLINKY_LEFT_2,
  TILE_BLINKY_UP_1,
  TILE_BLINKY_UP_2,
  TILE_BLINKY_DOWN_1,
  TILE_BLINKY_DOWN_2,
  TILE_PINKY_RIGHT_1,
  TILE_PINKY_RIGHT_2,
  TILE_PINKY_LEFT_1,
  TILE_PINKY_LEFT_2,
  TILE_PINKY_UP_1,
  TILE_PINKY_UP_2,
  TILE_PINKY_DOWN_1,
  TILE_PINKY_DOWN_2,
  TILE_INKY_RIGHT_1,
  TILE_INKY_RIGHT_2,
  TILE_INKY_LEFT_1,
  TILE_INKY_LEFT_2,
  TILE_INKY_UP_1,
  TILE_INKY_UP_2,
  TILE_INKY_DOWN_1,
  TILE_INKY_DOWN_2,
  TILE_CLYDE_RIGHT_1,
  TILE_CLYDE_RIGHT_2,
  TILE_CLYDE_LEFT_1,
  TILE_CLYDE_LEFT_2,
  TILE_CLYDE_UP_1,
  TILE_CLYDE_UP_2,
  TILE_CLYDE_DOWN_1,
  TILE_CLYDE_DOWN_2,
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
  GHOST_TYPE_BLINKY,
  GHOST_TYPE_PINKY,
  GHOST_TYPE_INKY,
  GHOST_TYPE_CLYDE,
};

enum PhaseType {
  PHASE_SCATTER,
  PHASE_CHASE,
};

enum GameState {
  GAME_STATE_READY,
  GAME_STATE_PLAYING,
  GAME_STATE_PAUSE,
  GAME_STATE_PACMAN_DIE,
  GAME_STATE_LEVEL_COMPLETE,
};

enum GhostState {
  GHOST_STATE_HOME,
  GHOST_STATE_EXITING,
  GHOST_STATE_CHASE,
  GHOST_STATE_SCATTER,
  GHOST_STATE_FRIGHTENED,
  GHOST_STATE_EATEN,
};

// ===================== STRUCTS =====================

struct GameContext;
struct AppContext;
struct Entity;

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

typedef struct Vec2 (*GhostTargetTile)(struct GameContext *game, struct Entity *ghost);

struct Entity {
  enum EntityType type;
  struct fVec2 pos;
  enum Direction curr_dir;
  enum Direction desired_dir;
  float delta;

  struct {
    enum TileType tiles[4];
    size_t len;
    size_t curr;
  } texture;

  union {
    struct {
      struct Vec2 scatter_target_tile;
      enum GhostState state;
      enum GhostType type;
      GhostTargetTile get_target_tile;
      float dots_to_leave_home;
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

  struct {
    float timers[8];
    int curr;
  } phases;

  struct {
    int total;
    int collected;
  } dots;
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
    float program_start;
    float game_start;
    float phase_start;
    float last_dot;
    float last_frame;
  } timers;
};

struct GameContext {
  struct Stats stats;
  struct Level level;
  enum GameState state;

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
