#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#include "defines.h"
#include "input.h"
#include "level.h"
#include "pacman.h"
#include "types.h"

void initialize_app_context(struct AppContext *app) {
  load_level(app, app->level_number);
  SDL_Texture *tileset = IMG_LoadTexture(app->renderer, TILESET_PATH);
  SDL_SetTextureScaleMode(tileset, SDL_SCALEMODE_NEAREST);
  initialize_pacman(&app->pacman);
  app->tileset = tileset;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  SDL_SetAppMetadata("Pacman", "0.0.1", NULL);
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  int width = SCALED_TILE_SIZE * LEVEL_WIDTH;
  int height = SCALED_TILE_SIZE * LEVEL_HEIGHT;
  struct AppContext *app = (struct AppContext *)SDL_calloc(1, sizeof(struct AppContext));
  if (!SDL_CreateWindowAndRenderer(TITLE, width, height, 0, &app->window, &app->renderer)) {
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
  static const float deltatime_need = 1.0f / FPS;
  float deltatime = (now - app->prev_frame_time) / 1000;
  if (deltatime < deltatime_need) return SDL_APP_CONTINUE;
  app->prev_frame_time = now;

  int w, h;
  SDL_GetWindowSize(app->window, &w, &h);
  SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
  SDL_RenderClear(app->renderer);

  SDL_FRect dstmap = {0, 0, w, h};
  SDL_FRect srcmap = {
    TILE_SIZE * LEVEL_OFFSET_X,
    TILE_SIZE * LEVEL_OFFSET_Y,
    TILE_SIZE * LEVEL_WIDTH,
    TILE_SIZE * LEVEL_HEIGHT,
  };
  SDL_RenderTexture(app->renderer, app->tileset, &srcmap, &dstmap);
  render_level(app->level, app->renderer, app->tileset);
  render_pacman(&app->pacman, app->renderer, app->tileset);
  move_pacman(app->level, &app->pacman, deltatime);

  SDL_RenderPresent(app->renderer);
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  switch (event->type) {
  case SDL_EVENT_QUIT:
    return SDL_APP_SUCCESS;
  case SDL_EVENT_KEY_DOWN:
    if (event->key.key == SDLK_ESCAPE) return SDL_APP_SUCCESS;
    break;
  }
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  struct AppContext *app = (struct AppContext *)appstate;
  SDL_free(app);
  SDL_DestroyTexture(app->tileset);
}
