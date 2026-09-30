#include <stdio.h>
#include <stdlib.h>

#include "SDL.h"
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"
#include "game_tools.h"
#include "model.h"

int main(int argc, char* argv[]) {
  SDL_Window* window;
  SDL_Renderer* renderer;

  /* Initialize SDL*/
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  /* Create the window where the moves are going to be played (drawing)*/
  window = SDL_CreateWindow("SDL_RenderClear", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, 888, 500,
                            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  if (!window) {
    fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
  }

  SDL_SetWindowTitle(window, "Takuzu (Pirate Mode)");
  /* SDL_SetWindowIcon(window, SDL_Surface* icon)*/

  /* Calling SDL_CreateRenderer in order to play moves for the following window
   */
  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_FAILURE;
  }

  Env* env = init(window, renderer, argc, argv);
  if (!env) {
    fprintf(stderr, "init failed\n");
    clean(window, renderer, env);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_FAILURE;
  }

  /* Render loop */
  SDL_Event e;
  bool quit = false;
  while (!quit) {
    /* Events management */
    while (SDL_PollEvent(&e)) {
      /* Processing events */
      quit = process(window, renderer, env, &e);
      if (quit) break;
    }
    /*Grey for the background*/
    SDL_SetRenderDrawColor(renderer, 0xA0, 0xA0, 0xA0, 0xFF);
    SDL_RenderClear(renderer);
    /* render all what you want */
    render(window, renderer, env);
    SDL_RenderPresent(renderer);
    SDL_Delay(DELAY);
  }

  /* clean your environment */
  clean(window, renderer, env);

  /* Always be sure to clean up */
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}
