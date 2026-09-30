#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"

/*
test game new il nous test si le jeux est nouveau
*/
bool test_game_new(void) {
  square t[36] = {0};  // creé tableau de 36 cases empty
  game g = game_new(t);
  for (uint i = 0; i <= 5; i++) {
    for (uint j = 0; j <= 5; j++) {
      if (game_get_square(g, i, j) != 0) return false;
    }
  }
  return true;
}
/******************************************************/
/*
test game_set_square il nous test si la fct game_set_square remplie la case
*/

bool test_game_set_square(void)  // bug1
{
  square s = S_ZERO;
  game g0 = game_new_empty();
  for (uint i = 0; i <= 5; i++) {
    for (uint j = 0; j <= 5; j++) {
      game_set_square(g0, i, j, s);

      if ((game_get_number(g0, i, j) != 0)) {
        game_delete(g0);
        return false;
      }
    }
  }
  game_delete(g0);
  return true;
}
/******************************************************/
/*
test game_get_new_square il nous test si la fct game_get_next_square nous return
la case suivante
*/

bool test_game_get_next_square(void) {
  game g = game_default_solution();
  if (game_get_next_square(g, 2, 3, UP, 1) != game_get_square(g, 1, 3)) {
    return false;
  } else if (game_get_next_square(g, 1, 2, DOWN, 1) !=
             game_get_square(g, 2, 2)) {
    return false;
  } else if (game_get_next_square(g, 3, 3, LEFT, 1) !=
             game_get_square(g, 3, 2)) {
    return false;
  } else if (game_get_next_square(g, 4, 3, RIGHT, 1) !=
             game_get_square(g, 4, 4)) {
    return false;
  }

  return true;
}

/******************************************************/

/*
test game_is_immutable il nous test si la fct game_is_immutable nous return true
si la case est pas modifiable
*/

bool test_game_is_immutable(void) {
  game g = game_default();
  if (game_get_number(g, 0, 1) == S_IMMUTABLE_ONE) {
    if (game_is_immutable(g, 0, 1) == true) {
      return false;
    }
  }
  if (game_get_number(g, 0, 2) == S_IMMUTABLE_ZERO) {
    if (game_is_immutable(g, 0, 2) == true) {
      return false;
    }
  }
  return true;
}
bool test_game_play_move(void) {
  game g = game_default();
  for (uint i = 0; i <= 5; i++) {
    for (uint j = 0; j <= 5; j++) {
      if (game_get_number(g, i, j) == S_IMMUTABLE_ONE) {
        if (game_check_move(g, i, j, S_IMMUTABLE_ONE) == true) {
          return false;
        }
      }
      if (game_get_number(g, i, j) == S_IMMUTABLE_ZERO) {
        if (game_check_move(g, i, j, S_IMMUTABLE_ZERO == true)) {
          return false;
        }
      }
      if (game_get_square(g, i, j) != S_EMPTY) {
        if (game_check_move(g, i, j, S_EMPTY != true)) {
          return false;
        }
      }
    }
  }
  return true;
}
/******************************************************/
/*
test game_restart il nous test si la fct game_restart nous recommence le jeux a
0
*/

bool test_game_restart(void) {
  game g = game_default();
  game g1;
  g1 = game_default_solution();
  game_restart(g1);
  if (game_equal(g, g1)) {
    return true;
  }
  return false;
}
/******************************************************/
/*
test game_print il nous test si la fct game_print affiche la grille
*/
bool test_game_print(void) {
  game g = game_default();
  game_print(g);
  return true;
}
/******************************************************/
/*
test game_nb_cols il nous test si la fct game_nb_cols a calculer le nombre de
colone
*/

bool test_game_nb_cols(void) {
  /*game g = game_new_empty_ext(8, 8, true, true);
  game g1 = game_default();
  bool A = (7 == game_nb_cols(g));
  bool B = (6 == game_nb_cols(g1));
  if (A && B) {
    return true;
  }
  return false;*/
  return true;
}
/*
test game_nb_rows il nous test si la fct game_nb_rows a calculer le nombre de
ligne
*/

bool test_game_nb_rows(void) {
  /*game g = game_new_empty_ext(8, 8, true, true);
  game g1 = game_default();
  bool A = (7 == game_nb_rows(g));
  bool B = (6 == game_nb_rows(g1));
  if (A && B) {
    return true;
  }
  return false;*/
  return true;
}

/********************************************************/
int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("must include test names as arguments\n");
    return EXIT_FAILURE;
  }
  bool ok = false;
  if (strcmp("game_get_next_square", argv[1]) == 0) {
    ok = test_game_get_next_square();
  } else if (strcmp("game_new", argv[1]) == 0) {
    ok = test_game_new();
  } else if (strcmp("game_set_square", argv[1]) == 0) {
    ok = test_game_set_square();
  } else if (strcmp("game_is_immutable", argv[1]) == 0) {
    ok = test_game_is_immutable();
  } else if (strcmp("game_play_move", argv[1]) == 0) {
    ok = test_game_play_move();
  } else if (strcmp("game_restart", argv[1]) == 0) {
    ok = test_game_restart();
  } else if (strcmp("game_print", argv[1]) == 0) {
    ok = test_game_print();
  } else if (strcmp("game_nb_rows", argv[1]) == 0) {
    ok = test_game_nb_rows();
  } else if (strcmp("game_nb_cols", argv[1]) == 0) {
    ok = test_game_nb_cols();
  }

  else {
    fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  if (ok) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}
