#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "text.h"

void render_text(const char *text, size_t len, TTF_Font *font, SDL_Color color, struct fVec2 pos, SDL_Renderer *renderer) {
  SDL_Surface *surface = TTF_RenderText_Blended(font, text, len, color);
  SDL_Texture *texture;
  if (surface) {
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
  }
  if (!texture) {
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to create text texture\n");
    SDL_Quit();
  }
  SDL_FRect dstrect = {
    pos.x,
    pos.y,
    0,
    0,
  };
  SDL_GetTextureSize(texture, &dstrect.w, &dstrect.h);
  SDL_RenderTexture(renderer, texture, NULL, &dstrect);
  SDL_DestroyTexture(texture);
}
