#ifndef TEXT_INCLUDED
#define TEXT_INCLUDED

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stddef.h>

#include "types.h"

void render_text(const char *text, size_t len, TTF_Font *font, SDL_Color color, struct Align align, SDL_Renderer *renderer);

#endif
