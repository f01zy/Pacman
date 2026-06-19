#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdlib.h>

#include "defines.h"
#include "text.h"
#include "types.h"

void render_text(const char *text, size_t len, TTF_Font *font, SDL_Color color, struct Align align, SDL_Renderer *renderer) {
  SDL_Surface *surface = TTF_RenderText_Solid(font, text, len, color);
  SDL_Texture *texture;
  if (surface) {
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
  }
  if (!texture) {
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to create text texture: %s\n", SDL_GetError());
    SDL_Quit();
    exit(1);
  }
  float w, h;
  SDL_GetTextureSize(texture, &w, &h);
  struct fVec2 pos;

  if (align.h == HALIGN_LEFT) {
    pos.x = 0;
  } else if (align.h == HALIGN_CENTER) {
    pos.x = TILE_SIZE * LEVEL_WIDTH / 2.0f - w / 2.0f;
  } else {
    pos.x = TILE_SIZE * LEVEL_WIDTH - w;
  }

  if (align.v == VALIGN_TOP) {
    pos.y = 0;
  } else if (align.v == VALIGN_CENTER) {
    pos.y = TILE_SIZE * LEVEL_HEIGHT + TILE_SIZE * HUD_HEIGHT / 2.0f - h / 2.0f;
  } else {
    pos.y = TILE_SIZE * LEVEL_HEIGHT + TILE_SIZE * HUD_HEIGHT - h;
  }

  SDL_FRect dstrect = {
    pos.x,
    pos.y,
    w,
    h,
  };
  SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
  SDL_SetRenderScale(renderer, TILE_SCALE, TILE_SCALE);
  SDL_RenderTexture(renderer, texture, NULL, &dstrect);
  SDL_SetRenderScale(renderer, 1, 1);
  SDL_DestroyTexture(texture);
}
