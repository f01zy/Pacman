#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "defines.h"
#include "hud.h"
#include "input.h"
#include "level.h"
#include "pacman.h"
#include "types.h"

void load_resources(struct AppContext *app) {
  // Tileset
  SDL_Texture *tileset = IMG_LoadTexture(app->renderer, TILESET_PATH);
  SDL_SetTextureScaleMode(tileset, SDL_SCALEMODE_NEAREST);
  app->resources.tileset = tileset;

  // Font
  TTF_Font *font = TTF_OpenFont(FONT_PATH, TILE_SIZE);
  app->resources.font = font;
}

void initialize_app_context(struct AppContext *app) {
  load_level(&app->level);
  load_resources(app);
  initialize_pacman(&app->pacman);
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  SDL_SetAppMetadata("Pacman", "0.0.1", NULL);
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  int w = SCALED_TILE_SIZE * LEVEL_WIDTH;
  int h = SCALED_TILE_SIZE * (LEVEL_HEIGHT + HUD_HEIGHT);
  struct AppContext *app = (struct AppContext *)SDL_calloc(1, sizeof(struct AppContext));
  if (!SDL_CreateWindowAndRenderer(TITLE, w, h, 0, &app->window, &app->renderer)) {
    SDL_Log("Failed to create window and renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  initialize_app_context(app);
  *appstate = app;
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  struct AppContext *app = (struct AppContext *)appstate;
  handle_keyboard(app);
  float now = SDL_GetTicks();
  static const float need = 1.0f / FPS;
  float deltatime = (now - app->time.prev) / 1000;
  if (deltatime < need) return SDL_APP_CONTINUE;
  app->time.prev = now;
  app->time.delta = deltatime;

  SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
  SDL_RenderClear(app->renderer);

  iterate_hud(app);
  iterate_level(app);
  iterate_pacman(app);

  SDL_RenderPresent(app->renderer);
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
  struct AppContext *app = (struct AppContext *)appstate;
  SDL_free(app);
  SDL_DestroyTexture(app->resources.tileset);
  TTF_CloseFont(app->resources.font);
}
