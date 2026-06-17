#ifndef UTILITY_INCLUDED
#define UTILITY_INCLUDED

#include "types.h"

struct Entity *get_pacman(struct GameContext *game);
struct fVec2 get_tile_center_offset(struct fVec2 pos);

#endif
