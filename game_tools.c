#include "game_tools.h"

#include <assert.h>
#include <getopt.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"

/*****************************************/
game game_load(char* filename) {
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    return game_default();
  }
  int nbRows, nbCols, wrapping, unique;
  // Use fscanf to read the values from the file
  if (fscanf(file, "%d %d %d %d", &nbRows, &nbCols, &wrapping, &unique) != 4) {
    fclose(file);
    return game_default();
  }
  if (!(nbRows >= 2 && nbRows <= 12 && (nbRows % 2 == 0))) exit(EXIT_FAILURE);
  if (!(nbCols >= 2 && nbCols <= 12 && (nbCols % 2 == 0))) exit(EXIT_FAILURE);

  // Allocate memory for squares and check for errors
  square* squares = malloc(nbRows * nbCols * sizeof(square));
  if (squares == NULL) exit(EXIT_FAILURE);

  int i = 0;
  int c = 0;
  while (i < nbRows * nbCols && (c = fgetc(file)) != EOF) {
    if (c != 10) {
      char position = (char)c;
      switch (position) {
        case 'w':
          squares[i] = S_ZERO;
          break;
        case 'b':
          squares[i] = S_ONE;
          break;
        case 'e':
          squares[i] = S_EMPTY;
          break;
        case 'W':
          squares[i] = S_IMMUTABLE_ZERO;
          break;
        case 'B':
          squares[i] = S_IMMUTABLE_ONE;
          break;
        default:
          squares[i] = S_EMPTY;
          break;
      }
      i++;
    }
  }
  fclose(file);
  // If we didn't read the expected number of squares, free memory and exit
  // with an error
  if (i != nbRows * nbCols) {
    free(squares);
    exit(EXIT_FAILURE);
  }

  return game_new_ext(nbRows, nbCols, squares, wrapping, unique);
}
void game_save(cgame g, char* filename) {
  assert(g);
  FILE* fileSave = fopen(filename, "w");
  fprintf(fileSave, "%d %d %d %d\n", g->nb_rows, g->nb_cols, g->wrapping,
          g->unique);
  char po = ' ';
  for (int i = 0; i < g->nb_rows; i++) {
    for (int j = 0; j < g->nb_cols; j++) {
      square s = game_get_square(g, i, j);
      if (s == S_ZERO)
        po = 'w';
      else if (s == S_ONE)
        po = 'b';
      else if (s == S_IMMUTABLE_ZERO)
        po = 'W';
      else if (s == S_IMMUTABLE_ONE)
        po = 'B';
      else if (s == S_EMPTY) {
        po = 'e';
      }
      fprintf(fileSave, "%c", po);
    }
    fprintf(fileSave, "\n");
  }
  fclose(fileSave);
}
/**********************************/
game solve_rec(game g, int i, int j, int nb_empty) {
  game g_copy = game_copy(g);
  game solution = NULL;
  uint nbcols = game_nb_cols(g);
  uint nbrows = game_nb_rows(g);
  if (game_has_error(g, i, j)) {
    game_delete(g_copy);
    return NULL;
  }
  if (nb_empty == 0) {
    return g_copy;
  }

  // Trouver la première case vide
  while (i < nbrows && game_get_square(g_copy, i, j) != S_EMPTY) {
    if (j == nbcols - 1) {
      j = 0;
      i++;
    } else {
      j++;
    }
  }

  if (i >= nbrows) {
    return g_copy;
  }

  nb_empty--;

  // Essayer de résoudre avec un 1
  game_set_square(g_copy, i, j, S_ONE);
  solution = solve_rec(g_copy, i, j, nb_empty);

  if (solution == NULL) {
    // Si ça ne marche pas, essayer avec un 0
    game_set_square(g_copy, i, j, S_ZERO);
    solution = solve_rec(g_copy, i, j, nb_empty);
  }

  game_delete(g_copy);
  return solution;
}

bool game_solve(game g) {
  int nb_empty = 0;
  for (int i = 0; i < game_nb_rows(g); i++) {
    for (int j = 0; j < game_nb_cols(g); j++) {
      if (game_is_empty(g, i, j)) {
        nb_empty++;
      }
    }
  }
  game sol = solve_rec(g, 0, 0, nb_empty);
  if (sol != NULL) {
    for (int i = 0; i < game_nb_rows(g); i++) {
      for (int j = 0; j < game_nb_cols(g); j++) {
        if (game_is_empty(g, i, j)) {
          game_set_square(g, i, j, game_get_square(sol, i, j));
        }
      }
    }
    game_delete(sol);
    return true;
  }
  return false;
}

uint build_nb_rec(game g, uint i, uint j, uint* num_solutions) {
  uint c1 = 0;
  uint c2 = 0;
  uint nbcols = game_nb_cols(g);
  uint nbrows = game_nb_rows(g);
  if (j == nbcols) {
    i++;
    j = 0;
  }

  while (game_is_immutable(g, i, j)) {
    if ((j + 1) < nbcols) {
      j++;
    } else if (((j + 1) == nbcols) && ((i + 1) < nbrows)) {
      i++;
      j = 0;
    } else {  // dernière case du tableau
      if (game_is_over(g)) {
        (*num_solutions)++;
      }
      return (uint)0;
    }
  }
  // dernière case du tableau
  if (((j + 1) == nbcols) && ((i + 1) == nbrows)) {
    game_set_square(g, i, j, S_ONE);
    if (game_is_over(g)) {
      (*num_solutions)++;
      c1 = (uint)1;
    }
    game_set_square(g, i, j, S_ZERO);
    if (game_is_over(g)) {
      (*num_solutions)++;
      c2 = (uint)1;
    }
  } else {  // toutes les autres cases
    game_set_square(g, i, j, S_ONE);
    if (game_has_error(g, i, j) == 0) {
      c1 = build_nb_rec(g, i, j + 1, num_solutions);
    }
    game_set_square(g, i, j, S_ZERO);
    if (game_has_error(g, i, j) == 0) {
      c2 = build_nb_rec(g, i, j + 1, num_solutions);
    }
  }
  game_set_square(g, i, j, S_EMPTY);
  return (c1 + c2);
}

uint game_nb_solutions(cgame g) {
  uint x = 0;
  uint y = 0;
  uint num_solutions = 0;
  game g_copy = game_copy(g);
  uint ret = build_nb_rec(g_copy, x, y, &num_solutions);
  game_delete(g_copy);
  printf("Il y a %d solution(s).\n", num_solutions);
  printf("Il y a %d solution(s).\n", ret);
  return num_solutions;
}
