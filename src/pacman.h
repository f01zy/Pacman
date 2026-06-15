#ifndef PACMAN_INCLUDED
#define PACMAN_INCLUDED

#include <SDL3/SDL.h>

#include "defines.h"
#include "types.h"

void initialize_pacman(struct Pacman *pacman);
void change_pacman_direction(struct Pacman *pacman, enum Direction dir);
void render_pacman(struct Pacman *pacman, SDL_Renderer *renderer, SDL_Texture *tileset);
void move_pacman(enum TileType level[LEVEL_HEIGHT][LEVEL_WIDTH], struct Pacman *pacman, float deltatime);

#endif
