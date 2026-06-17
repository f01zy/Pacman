#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "defines.h"
#include "hud.h"
#include "text.h"
#include "types.h"

void render_hud(struct Stats *stats, const struct Resources *resources, struct SDL_Renderer *renderer) {
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
    struct Align align = {HALIGN_CENTER, VALIGN_CENTER};
    render_text(buf, len, resources->font, color, align, renderer);
  }
}

void iterate_hud(struct State *state) { render_hud(&state->game->stats, &state->app->resources, state->app->renderer); }
