#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stddef.h>

#include "defines.h"
#include "hud.h"
#include "text.h"
#include "types.h"

void render_hud(const struct Resources *resources, struct Stats *stats, struct SDL_Renderer *renderer) {
  if (stats->is_changed) {
    stats->is_changed = false;

    // Clear previos frame
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_FRect hud_rect = {
      0,
      SCALED_TILE_SIZE * LEVEL_HEIGHT,
      SCALED_TILE_SIZE * LEVEL_WIDTH,
      SCALED_TILE_SIZE * HUD_HEIGHT,
    };
    SDL_RenderFillRect(renderer, &hud_rect);

    // Draw score text
    char buf[MAX_TEXT_BUFFER_SIZE];
    size_t len = SDL_snprintf(buf, sizeof(buf), "Score: %d", stats->score);
    SDL_Color color = {255, 255, 255, 255};
    struct fVec2 pos = {
      SCALED_TILE_SIZE * LEVEL_WIDTH / 2.0f,
      SCALED_TILE_SIZE * LEVEL_HEIGHT + SCALED_TILE_SIZE * HUD_HEIGHT / 2.0f,
    };
    render_text(buf, len, resources->font, color, pos, renderer);
  }
}

void iterate_hud(struct AppContext *app) { render_hud(&app->resources, &app->stats, app->renderer); }
