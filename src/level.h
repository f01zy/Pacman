#ifndef MAP_INCLUDED
#define MAP_INCLUDED

#include <SDL3/SDL.h>

#include "types.h"

void load_level(struct Level *level);
void iterate_level(struct AppContext *app);

#endif
