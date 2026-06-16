#ifndef TEXT_INCLUDED
#define TEXT_INCLUDED

#include "types.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stddef.h>

void render_text(const char *text, size_t len, TTF_Font *font, SDL_Color color, struct fVec2 pos, SDL_Renderer *renderer);

#endif
