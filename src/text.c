#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "defines.h"
#include "text.h"
#include "types.h"

void render_text(const char *text, size_t len, TTF_Font *font, SDL_Color color, struct fVec2 pos, SDL_Renderer *renderer) {
  SDL_Surface *surface = TTF_RenderText_Solid(font, text, len, color);
  SDL_Texture *texture;
  if (surface) {
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
  }
  if (!texture) {
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to create text texture: %s\n", SDL_GetError());
    SDL_Quit();
  }
  float w, h;
  SDL_GetTextureSize(texture, &w, &h);
  SDL_FRect dstrect = {
    pos.x / TILE_SCALE - w / 2,
    pos.y / TILE_SCALE - h / 2,
    w,
    h,
  };
  SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
  SDL_SetRenderScale(renderer, TILE_SCALE, TILE_SCALE);
  SDL_RenderTexture(renderer, texture, NULL, &dstrect);
  SDL_SetRenderScale(renderer, 1, 1);
  SDL_DestroyTexture(texture);
}
