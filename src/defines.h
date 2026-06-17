#ifndef DEFINES_INCLUDED
#define DEFINES_INCLUDED

// Game constans
#define TILE_SIZE                8
#define TILE_SCALE               2
#define SCALED_TILE_SIZE         (TILE_SIZE * TILE_SCALE)
#define TILESET_PATH             "/home/aminov/Documents/Programming/Pacman/resources/tileset.png"
#define FONT_PATH                "/home/aminov/Documents/Programming/Pacman/resources/font.ttf"

#define PACMAN_SPEED             120
#define PACMAN_START_X           13
#define PACMAN_START_Y           17

#define DOT_SCORE                10
#define ENERGIZER_SCORE          50
#define OFFSET_ALLOWS_CHANGE_DIR 1
#define ENTITY_ANIMATION_SPEED   15

// UI
#define TITLE                    "Pacman"
#define LEVEL_OFFSET_X           0
#define LEVEL_OFFSET_Y           0
#define LEVEL_WIDTH              28
#define LEVEL_HEIGHT             31
#define HUD_HEIGHT               6

// Graphics
#define FPS                      60

// Utility
#define MAX_TEXT_BUFFER_SIZE     256

#endif
