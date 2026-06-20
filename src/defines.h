#ifndef DEFINES_INCLUDED
#define DEFINES_INCLUDED

// UI
#define TITLE                    "Pacman"
#define LEVEL_WIDTH              28
#define LEVEL_HEIGHT             31
#define HUD_HEIGHT               6
#define PACMAN_DIE_TILES         11

// Game constans
#define MAX_SPEED                80
#define ENTITY_ANIMATION_SPEED   15
#define TILE_SIZE                8
#define TILE_SCALE               2
#define SCALED_TILE_SIZE         (TILE_SIZE * TILE_SCALE)
#define TILESET_PATH             "/home/aminov/Documents/Programming/Pacman/resources/tileset.png"
#define FONT_PATH                "/home/aminov/Documents/Programming/Pacman/resources/font.ttf"
#define GAME_READY_TIMER         4.5f
#define GHOST_INACTIVITY_TIMER   4.0f
#define DOTS_COUNT               244
#define DOT_SCORE                10
#define ENERGIZER_SCORE          50
#define OFFSET_ALLOWS_CHANGE_DIR 3
#define CLYDE_RADIUS             8.0f
#define GHOST_HOME_TARGET_X      13
#define GHOST_HOME_TARGET_Y      11
#define PACMAN_LIVES             3

// Graphics
#define FPS                      60

// Utility
#define MIN(A, B)                ((A) < (B) ? (A) : (B))
#define MAX(A, B)                ((A) > (B) ? (A) : (B))
#define MAX_TEXT_BUFFER_SIZE     256

#endif
