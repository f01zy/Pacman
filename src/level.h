#ifndef MAP_INCLUDED
#define MAP_INCLUDED

#include <SDL3/SDL.h>

#include "defines.h"
#include "types.h"

void load_level(struct AppContext *app, int level);
void render_level(enum TileType level[LEVEL_HEIGHT][LEVEL_WIDTH], SDL_Renderer *renderer, SDL_Texture *tileset);

#endif
