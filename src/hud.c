#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "defines.h"
#include "hud.h"
#include "text.h"
#include "types.h"
#include "utility.h"

void render_hud(struct GameContext *game, const struct Resources *resources, struct SDL_Renderer *renderer) {
  if (!game->stats.is_changed) return;
  game->stats.is_changed = false;

  // Clear previous frame
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_FRect hud_rect = {
    0,
    SCALED_TILE_SIZE * LEVEL_HEIGHT,
    SCALED_TILE_SIZE * LEVEL_WIDTH,
    SCALED_TILE_SIZE * HUD_HEIGHT,
  };
  SDL_RenderFillRect(renderer, &hud_rect);

  // Render lives
  for (int i = 0; i < game->lives - 1; i++) {
    struct Tile tile = get_tile(TILE_PACMAN_LEFT_2);
    SDL_FRect dstrect = {
      SCALED_TILE_SIZE * tile.size.x * i,
      SCALED_TILE_SIZE * (LEVEL_HEIGHT + HUD_HEIGHT / 2.0f - tile.size.y / 2.0f),
      SCALED_TILE_SIZE * tile.size.x,
      SCALED_TILE_SIZE * tile.size.y,
    };
    SDL_FRect srcrect = {
      TILE_SIZE * tile.offset.x,
      TILE_SIZE * tile.offset.y,
      TILE_SIZE * tile.size.x,
      TILE_SIZE * tile.size.y,
    };
    SDL_RenderTexture(renderer, resources->tileset, &srcrect, &dstrect);
  }

  // Render score
  size_t len;
  char buf[MAX_TEXT_BUFFER_SIZE];
  SDL_Color color = (struct SDL_Color){255, 255, 255, 255};
  if (game->state == GAME_STATE_GAME_OVER) {
    color = (struct SDL_Color){255, 0, 0, 255};
    len = SDL_snprintf(buf, sizeof(buf), "Game Over");
  } else if (game->state == GAME_STATE_READY) {
    len = SDL_snprintf(buf, sizeof(buf), "READY!");
  } else {
    len = SDL_snprintf(buf, sizeof(buf), "Score: %d", game->stats.score);
  }
  struct Align align = {HALIGN_CENTER, VALIGN_CENTER};
  render_text(buf, len, resources->font, color, align, renderer);
}

void iterate_hud(struct State *state) { render_hud(state->game, &state->app->resources, state->app->renderer); }
