#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"

bool test_game_new_empty(void) {
  game g = game_new_empty();

  int i, j;
  for (i = 0; i <= 5; i++) {
    for (j = 0; j <= 5; j++) {
      if (game_get_square(g, i, j) != S_EMPTY) return false;
    }
  }
  return true;
}

/**********************************************************/
bool test_game_delete(void) {
  // game g = game_default();
  game g = game_default_solution();
  game_delete(g);
  if (g != game_new_empty()) return false;

  return true;
}

/*************************************************************/
bool test_game_get_number(void) {
  square s = S_ONE;
  square s2 = S_ZERO;
  square s3 = S_EMPTY;
  game g1 = game_new_empty();
  int i, j;
  for (i = 0; i <= 5; i++) {
    for (j = 0; j <= 5; j++) {
      game_play_move(g1, i, j, s);
      if (1 != game_get_number(g1, i, j)) return false;
      game g2 = game_new_empty();
      game_play_move(g2, i, j, s2);
      if (0 != game_get_number(g2, i, j)) return false;
      game g3 = game_new_empty();
      game_play_move(g3, i, j, s3);
      if (-1 != game_get_number(g3, i, j)) return false;
    }
  }

  return true;
}

/*********************************************************/
bool test_game_is_empty() {
  game g = game_default();
  game_play_move(g, 1, 0, S_ZERO);

  if (game_is_empty(g, 0, 0) == true && game_is_empty(g, 1, 0) == false &&
      game_is_empty(g, 0, 1) == false) {
    return true;
  }
  return false;
}

/*********************************************************/
bool test_game_check_move(void) {
  game g = game_default();
  if (game_check_move(g, 7, 7, S_ONE) != false) return false;
  if (game_check_move(g, 4, 4, S_EMPTY) != true) return false;
  if (game_check_move(g, 0, 1, S_IMMUTABLE_ONE) != false) return false;

  return true;
}

/********************************************************/

bool test_game_default_solution(void) {
  game g = game_default_solution();
  if (game_is_over(g) != true) return false;

  return true;
}
/***********************ext_tests***************************/
/***********************************************************/
bool test_game_new_ext(void) { return true; }

/**********************************************************/
bool test_game_is_unique(void) {
  game g = game_new_empty_ext(8, 8, true, true);  // avec unique option
  game g1 = game_default();                       // no unique option
  if (game_is_unique(g) && !game_is_unique(g1)) {
    return true;
  }
  return false;
}

/**********************************************************/

game monjeux(void) {
  game g1 = game_default();

  game_play_move(g1, 1, 1, 1);
  game_play_move(g1, 5, 0, 2);

  return g1;
}

game monjeuxExt(void) {
  game g1 = game_new_empty_ext(8, 8, true, true);
  return g1;
}

/**********************************************************/
/**********************************************************/

int main(int argc, char *argv[]) {
  /******main******/
  if (argc == 1) {
    printf("must include test names as arguments\n");
    return EXIT_FAILURE;
  }
  bool ok = false;
  if (strcmp("game_new_empty", argv[1]) == 0) {
    ok = test_game_new_empty();
  } else if (strcmp("game_delete", argv[1]) == 0) {
    ok = test_game_delete();
  } else if (strcmp("game_get_number", argv[1]) == 0) {
    ok = test_game_get_number();
  } else if (strcmp("game_is_empty", argv[1]) == 0) {
    ok = test_game_is_empty();
  } else if (strcmp("game_check_move", argv[1]) == 0) {
    ok = test_game_check_move();
  } else if (strcmp("game_default_solution", argv[1]) == 0) {
    ok = test_game_default_solution();
  } else if (strcmp("game_new_ext", argv[1]) == 0) {
    ok = test_game_new_ext();
  } else if (strcmp("game_is_unique", argv[1]) == 0) {
    ok = test_game_is_unique();
  } else {
    fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
    exit(EXIT_FAILURE);
  }
  if (ok) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAadl ILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}

/**********************************************************/
/*******TEST FOR GAME SAVE AND GAME LOAD*******************/
/**********************************************************/
