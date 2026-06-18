#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "defines.h"
#include "entity.h"
#include "events.h"
#include "hud.h"
#include "input.h"
#include "level.h"
#include "types.h"

void load_resources(struct AppContext *app) {
  SDL_Texture *tileset = IMG_LoadTexture(app->renderer, TILESET_PATH);
  SDL_SetTextureScaleMode(tileset, SDL_SCALEMODE_NEAREST);
  app->resources.tileset = tileset;
  TTF_Font *font = TTF_OpenFont(FONT_PATH, TILE_SIZE);
  app->resources.font = font;
}

void initialize_state(struct State *state) {
  state->game->state = GAME_STATE_READY;
  state->game->stats.is_changed = true;
  initialize_entities(state->game);
  load_level(&state->game->level);
  load_resources(state->app);
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  SDL_SetAppMetadata("Pacman", "0.0.1", NULL);

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Failed to initialize SDL: %s\n", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  if (!TTF_Init()) {
    SDL_Log("Failed to initialize SDL_ttf: %s\n", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  int w = SCALED_TILE_SIZE * LEVEL_WIDTH;
  int h = SCALED_TILE_SIZE * (LEVEL_HEIGHT + HUD_HEIGHT);
  struct AppContext *app = (struct AppContext *)SDL_calloc(1, sizeof(struct AppContext));
  struct GameContext *game = (struct GameContext *)SDL_calloc(1, sizeof(struct GameContext));
  struct State *state = (struct State *)SDL_calloc(1, sizeof(struct State));
  state->app = app;
  state->game = game;

  if (!SDL_CreateWindowAndRenderer(TITLE, w, h, 0, &app->window, &app->renderer)) {
    SDL_Log("Failed to create window and renderer: %s\n", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  initialize_state(state);
  *appstate = state;
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  struct State *state = (struct State *)appstate;
  handle_keyboard(state->game);

  if (state->game->state == GAME_STATE_PACMAN_DIE) {
    SDL_Log("You lose\n");
    return SDL_APP_SUCCESS;
  }
  if (state->game->state == GAME_STATE_LEVEL_COMPLETE) {
    SDL_Log("You win\n");
    return SDL_APP_SUCCESS;
  }

  float now = SDL_GetTicks();
  static const float need = 1.0f / FPS;
  float deltatime = (now - state->app->time.prev) / 1000.0f;
  if (deltatime < need) return SDL_APP_CONTINUE;
  state->app->time.prev = now;
  state->app->time.delta = deltatime;

  iterate_events(state);
  iterate_level(state);
  iterate_hud(state);
  for (int i = 0; i < state->game->entities.len; i++) {
    iterate_entity(state, state->game->entities.buf[i]);
  }

  SDL_RenderPresent(state->app->renderer);
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  switch (event->type) {
  case SDL_EVENT_QUIT:
    return SDL_APP_SUCCESS;
  }
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  struct State *state = (struct State *)appstate;
  SDL_DestroyTexture(state->app->resources.tileset);
  TTF_CloseFont(state->app->resources.font);
  for (int i = 0; i < state->game->entities.len; i++) {
    SDL_free(state->game->entities.buf[i]);
  }
  SDL_free(state->game->entities.buf);
  SDL_free(state->game);
  SDL_free(state->app);
  SDL_free(state);
}
