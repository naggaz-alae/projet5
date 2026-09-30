#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_private.h"
#include "game_struct.h"
#include "game_tools.h"

#define USAGE1 "Usage 1: %s <option> <input> [<output>]\n"
#define USAGE2 "Usage 2: %s <option> <input> [<output>]\n"
#define NO_SOL "No solution found\n"

int main(int argc, char* argv[]) {
  if (argc < 3) {
    fprintf(stderr, USAGE1, argv[0]);
    return EXIT_FAILURE;
  }

  char* fdsolution = "-s";
  char* ctsolution = "-c";

  char* fileIn = argv[2];
  game g = game_load(fileIn);

  game_print(g);

  char* fileOut = argv[3];

  if (strcmp(argv[1], fdsolution) == 0) {
    if (game_solve(g)) {
      if (argc == 4) {
        game_save(g, fileOut);
      } else {
        game_print(g);
      }
    } else {
      printf(NO_SOL);
      return EXIT_FAILURE;
    }
  }

  else if (strcmp(argv[1], ctsolution) == 0) {
    int nbsolution = game_nb_solutions(g);
    if (argc == 4) {
      FILE* fptr = fopen(fileOut, "w");
      fprintf(fptr, "%d\n", nbsolution);
      fclose(fptr);

    } else {
      printf("%d\n", nbsolution);
    }
  }

  else {
    fprintf(stderr, USAGE2, argv[0]);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
