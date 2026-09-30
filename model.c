// SDL2 Demo by aurelien.esnard@u-bordeaux.fr

#include "model.h"

#include <SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_aux.c"
#include "game_ext.h"
#include "game_tools.h"

/* **************************************************************** */

#define BACKGROUND "MainBackground.jpg"
#define EMPTY "empty.jpg"
#define HELP "Help.jpg"
#define ERREUR "erreur.jpg"
#define SQUAREB "squareB.jpg"
#define SQUAREW "squareW.jpg"
#define IMMUTABLEB "immutableB.jpg"
#define IMMUTABLEW "immutableW.jpg"
struct Env_t {
  /* PUT YOUR VARIABLES HERE */
  SDL_Texture *background;
  SDL_Texture *empty;
  SDL_Texture *immutableB;
  SDL_Texture *immutableW;
  SDL_Texture *squareB;
  SDL_Texture *squareW;

  SDL_Texture *help;
  SDL_Texture *erreur;
  game g;
  bool displayhelp;
};

/* **************************************************************** */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[]) {
  Env *env = malloc(sizeof(struct Env_t));
  if (argc == 2) {
    char *filename = (char *)argv[1];
    env->g = game_load(filename);
  } else {
    env->g = game_default();
  }

  char *textureFilenames[] = {BACKGROUND, EMPTY,   HELP,       ERREUR,
                              SQUAREW,    SQUAREB, IMMUTABLEW, IMMUTABLEB};
  SDL_Texture **textures[] = {&env->background, &env->empty,     &env->help,
                              &env->erreur,     &env->squareW,   &env->squareB,
                              &env->immutableW, &env->immutableB};

  for (int i = 0; i < sizeof(textureFilenames) / sizeof(textureFilenames[0]);
       i++) {
    *(textures[i]) = IMG_LoadTexture(ren, textureFilenames[i]);
    if (!*(textures[i])) ERROR("IMG_LoadTexture: %s\n", textureFilenames[i]);
  }

  env->displayhelp = false;

  /* PUT YOUR CODE HERE TO INIT TEXTURES, ... */
  return env;
}

/* **************************************************************** */

void render(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  /* get current window size */
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  // Set the viewport to the entire screen
  SDL_RenderSetViewport(ren, NULL);

  // Render the background image
  SDL_RenderCopy(ren, env->background, NULL, NULL);

  // Calculate the size of each square
  int image_width = 50;
  int image_height = 50;

  // Calculate the number of rows and columns in the game
  const int rows = game_nb_rows(env->g);
  const int cols = game_nb_cols(env->g);

  // Calculate the center of the screen
  int x = w / 2;
  int y = h / 2;

  // Render the game board
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      // Calculate the position of the square
      SDL_Rect rect = {(j * image_width) + x - ((cols / 2) * image_width),
                       (i * image_height) + y - ((rows / 2) * image_height),
                       image_width, image_height};

      // Get the value of the square
      square square = game_get_square(env->g, i, j);

      // Render the appropriate square image
      switch (square) {
        case S_ONE:
          SDL_RenderCopy(ren, env->squareB, NULL, &rect);
          break;
        case S_ZERO:
          SDL_RenderCopy(ren, env->squareW, NULL, &rect);
          break;
        case S_IMMUTABLE_ONE:
          SDL_RenderCopy(ren, env->immutableB, NULL, &rect);
          break;
        case S_IMMUTABLE_ZERO:
          SDL_RenderCopy(ren, env->immutableW, NULL, &rect);
          break;
        default:
          SDL_RenderCopy(ren, env->empty, NULL, &rect);
          break;
      }

      // Check if the square has an error and render the error image if
      // necessary
      if (game_has_error(env->g, i, j)) {
        SDL_RenderCopy(ren, env->erreur, NULL, &rect);
      }
    }
  }

  // Render the help image if necessary
  if (env->displayhelp) {
    SDL_RenderCopy(ren, env->help, NULL, NULL);
  }

  // Update the screen
  SDL_RenderPresent(ren);
}

/* **************************************************************** */

bool process(SDL_Window *win, SDL_Renderer *ren, Env *env, SDL_Event *e) {
  int w, h;
  SDL_GetWindowSize(win, &w, &h);
  int rows = game_nb_rows(env->g);
  int cols = game_nb_cols(env->g);
  int mouse_x, mouse_y;
  SDL_GetMouseState(&mouse_x, &mouse_y);
  int image_width = 50;
  int image_height = 50;

  if (e->type == SDL_QUIT || e->key.keysym.sym == SDLK_ESCAPE ||
      e->key.keysym.sym == SDLK_q) {
    return true;
  }

  if (e->type == SDL_KEYDOWN) {
    switch (e->key.keysym.sym) {
      case SDLK_h:
        env->displayhelp = !env->displayhelp;
        break;
      case SDLK_r:
        game_restart(env->g);
        break;
      case SDLK_z:
        game_undo(env->g);
        break;
      case SDLK_y:
        game_redo(env->g);
        break;
      case SDLK_SPACE:
        game_play_move(env->g, 0, 0, S_ZERO);
        break;
      case SDLK_w:
      case SDLK_b:
      case SDLK_e: {
        int stone = S_EMPTY;
        if (e->key.keysym.sym == SDLK_w) {
          stone = S_ZERO;
        } else if (e->key.keysym.sym == SDLK_b) {
          stone = S_ONE;
        }
        if (mouse_x > w / 2 - ((rows / 2) * image_width) &&
            mouse_x < w / 2 + ((rows / 2) * image_width) &&
            mouse_y > h / 2 - ((cols / 2) * image_height) &&
            mouse_y < h / 2 + ((cols / 2) * image_height)) {
          int x =
              (mouse_x - (w / 2 - ((rows / 2) * image_width))) / image_width;
          int y =
              (mouse_y - (h / 2 - ((cols / 2) * image_height))) / image_height;
          if (game_check_move(env->g, y, x, stone)) {
            game_play_move(env->g, y, x, stone);
          }
        }
        break;
      }
      case SDLK_s:
        game_solve(env->g);
        break;
    }
  }

  /* PUT YOUR CODE HERE TO PROCESS EVENTS */

  return false;
}

/* **************************************************************** */

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  SDL_DestroyTexture(env->background);
  SDL_DestroyTexture(env->empty);
  SDL_DestroyTexture(env->squareB);
  SDL_DestroyTexture(env->squareW);
  SDL_DestroyTexture(env->immutableB);
  SDL_DestroyTexture(env->immutableW);
  SDL_DestroyTexture(env->help);
  SDL_DestroyTexture(env->erreur);
  game_delete(env->g);
  free(env);
}

/* **************************************************************** */
