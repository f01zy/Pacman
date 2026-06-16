#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stddef.h>

#include "defines.h"
#include "hud.h"
#include "text.h"
#include "types.h"

void render_hud(const struct Resources *resources, const struct Stats *stats, struct SDL_Renderer *renderer) {
  char buf[MAX_TEXT_BUFFER_SIZE];
  size_t len = SDL_snprintf(buf, sizeof(buf), "%d", stats->score);
  SDL_Color color = {255, 255, 255, SDL_ALPHA_OPAQUE};
  struct fVec2 pos = {
    SCALED_TILE_SIZE * LEVEL_HEIGHT,
    0,
  };
  render_text(buf, len, resources->font, color, pos, renderer);
}

void iterate_hud(struct AppContext *app) { render_hud(&app->resources, &app->stats, app->renderer); }
