#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "defines.h"
#include "hud.h"
#include "text.h"
#include "types.h"

void render_hud(struct GameContext *game, const struct Resources *resources, struct SDL_Renderer *renderer) {
  if (game->stats.is_changed) {
    game->stats.is_changed = false;

    // Clear previos frame
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_FRect hud_rect = {
      0,
      SCALED_TILE_SIZE * LEVEL_HEIGHT,
      SCALED_TILE_SIZE * LEVEL_WIDTH,
      SCALED_TILE_SIZE * HUD_HEIGHT,
    };
    SDL_RenderFillRect(renderer, &hud_rect);

    // Draw hud
    char buf[MAX_TEXT_BUFFER_SIZE];
    size_t len;
    if (game->state == GAME_STATE_READY) {
      len = SDL_snprintf(buf, sizeof(buf), "READY!");
    } else {
      len = SDL_snprintf(buf, sizeof(buf), "Score: %d", game->stats.score);
    }
    SDL_Color color = {255, 255, 255, 255};
    struct Align align = {HALIGN_CENTER, VALIGN_CENTER};
    render_text(buf, len, resources->font, color, align, renderer);
  }
}

void iterate_hud(struct State *state) { render_hud(state->game, &state->app->resources, state->app->renderer); }
