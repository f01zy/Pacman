#ifndef MAP_INCLUDED
#define MAP_INCLUDED

#include <SDL3/SDL.h>

#include "types.h"

void load_level(struct State *state, bool is_update_map);
void iterate_level(struct State *state);

#endif
