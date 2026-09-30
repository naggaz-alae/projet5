#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

game game_tst(void) {
  game g1 = game_default();

  game_play_move(g1, 1, 1, 1);  // no erreur
  game_play_move(g1, 5, 0, 2);

  return g1;
}
bool test_game_copy(void)  // bug16
{
  game g1 = game_tst();
  game g2 = game_copy(g1);  // copie mon jeux dans g2
                            // comparé si g1 = g2
  for (int i = 0; i <= 5; i++) {
    for (int k = 0; k <= 5; k++) {
      if (game_get_square(g1, i, k) != game_get_square(g2, i, k)) {
        game_delete(g1);
        game_delete(g2);
        return false;
      }
    }
  }
  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_game_equal(void)  // bug5
{
  square t[36] = {1, 1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 2,
                  2, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 1};
  game g1 = game_new(t);  // cree 3 game pour teste tout les cas possibles
  // square t2[36] = {1, 1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 2,
  //                  2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  game g2 = game_new(t);
  // square t3[36] = {1, 1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 2,
  //                  2, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 1};
  game g3 = game_new(t);
  // si game equale entre g1 et g2 = false => g1!= g2
  if (game_equal(g1, g2) == false) {
    for (int i = 0; i <= 5; i++) {
      for (int k = 0; k <= 5; k++) {
        if (game_get_square(g1, i, k) == game_get_square(g2, i, k)) {
          game_delete(g1);
          game_delete(g2);
          return false;
        }
      }
    }
  }
  if (game_equal(g2, g3) ==
      true) {  // si game equale entre g3 et g2 = true => g3= g2
    for (int i = 0; i <= 5; i++) {
      for (int k = 0; k <= 5; k++) {
        if (game_get_square(g1, i, k) != game_get_square(g2, i, k)) {
          game_delete(g2);
          game_delete(g3);
          return false;
        }
      }
    }
  }
  game_delete(g1);
  game_delete(g2);
  game_delete(g3);
  return true;
}

bool test_game_get_square(void) {
  square s = S_ONE;
  square s2 = S_ZERO;
  square s3 = S_EMPTY;
  game g1 = game_new_empty();
  for (int i = 0; i <= 5; i++) {
    for (int k = 0; k <= 5; k++) {
      game_play_move(g1, i, k, s);  // joue s dans tous les cases de g1
      if (s != game_get_square(g1, i, k)) return false;
      game g2 = game_new_empty();
      game_play_move(g2, i, k, s2);  // joue s2 dans tous les cases de g1
      if (s2 != game_get_square(g2, i, k)) return false;
      game g3 = game_new_empty();
      game_play_move(g3, i, k, s3);  // joue s3 dans tous les cases de g1
      if (s3 != game_get_square(g3, i, k)) return false;
    }
  }

  return true;
}

bool test_game_get_next_number(void) {
  direction dir1 = DOWN;
  direction dir2 = UP;
  direction dir3 = LEFT;
  direction dir4 = RIGHT;

  game g = game_default_solution();

  if (dir1) {  // testé tous lec de la direction down
    if (game_get_next_number(g, 1, 2, DOWN, 1) != game_get_number(g, 2, 2) &&
        game_get_next_number(g, 1, 2, DOWN, 1) != 0)
      return false;
  }
  if (dir2) {  // testé tous lec de la direction up
    if (game_get_next_number(g, 2, 3, UP, 1) != game_get_number(g, 1, 3) &&
        game_get_next_number(g, 2, 3, UP, 1) != 0)
      return false;
  }
  if (dir3) {  // testé tous lec de la direction left
    if (game_get_next_number(g, 3, 2, LEFT, 1) != game_get_number(g, 3, 1) &&
        game_get_next_number(g, 3, 2, LEFT, 1) != 0)
      return false;
  }
  if (dir4) {  // testé tous lec de la direction right
    if (game_get_next_number(g, 4, 1, RIGHT, 2) != game_get_number(g, 4, 3) &&
        game_get_next_number(g, 4, 1, RIGHT, 2) != 0)
      return false;
  }
  return true;
}

bool test_game_has_error(void)  // bug14 resoudre suucces
{
  game g1 = game_default();
  game_play_move(g1, 1, 1,
                 S_ZERO);  // joué trois cases white Consécutif sur la colones 1
  game_play_move(g1, 2, 2,
                 S_ZERO);  // joué trois cases white Consécutif sur la ligne 2
  game_play_move(g1, 2, 3,
                 S_ZERO);  // joué trois cases white Consécutif sur la ligne 2

  int A = game_has_error(g1, 1, 1);  // erreur de trois cases Consécutif
  int B = game_has_error(g1, 0, 0);  // no erreur
  int C = game_has_error(g1, 5, 5);  // no erreur
  int D = game_has_error(g1, 2, 3);  // erreur de  trois cases white Consécutif
  if (A != 0 && B == 0 && C == 0 && D != 0) {
    return true;
  }

  return false;
}

bool test_game_is_over(void)  // bug12 regle //reste_13 success
{
  game g1 = game_default();
  game g3 = game_new_empty();
  game g4 = game_tst();
  square t[36] = {1, 1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 2,
                  2, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 1};
  game g2 = game_new(t);

  if (game_is_over(g4) != false) return false;  // game no succes
  if (game_is_over(g1) != false) return false;  // game no succes
  if (game_is_over(g2) != false) return false;  // game no succes
  if (game_is_over(g3) != false) return false;  // game no succes

  game g5 = game_default_solution();
  if (game_is_over(g5) != true) return false;  // game succes
  return true;
}
bool test_game_default(void) {
  game g = game_default();  // comparer les cases de game default
  if (game_get_square(g, 0, 1) != S_IMMUTABLE_ONE &&
      game_get_square(g, 0, 2) != S_IMMUTABLE_ZERO &&
      game_get_square(g, 0, 0) != S_EMPTY)
    return false;
  return true;
}

/**************************************************/

bool test_game_new_empty_ext(void) {
  /**game g = game_new_empty_ext(8, 8, true,
                              true);  // initialisé empty game de i=8 et j= 8
                                      // avec option wrappin et unique
  game g0 = game_new_empty_ext(6, 4, false,
                               false);  // initialisé emtu game de i=6 et j= 4
                                        // sans option wrappin et unique
  bool A = game_is_unique(g);           // testé si g et unique
  bool B = game_is_wrapping(g);         // testé si g et wrraping
  bool G = game_is_unique(g0);          // testé si g0 et unique
  bool H = game_is_wrapping(g0);        // testé si g0 et wrraping
  int C = game_nb_rows(g);              // recupere le nombre de rows de g
  int D = game_nb_cols(g);              // recupere le nombre de cils de g
  int E = game_nb_rows(g0);             // recupere le nombre de rows de g0
  int F = game_nb_cols(g0);             // recupere le nombre de cols de g0
  bool X = true;
  bool Y = true;
  for (uint i = 0; i < C; i++) {
    for (uint j = 0; j < D; j++) {
      if (game_get_square(g, i, j) !=
          S_EMPTY) {  // testé si tous les cases de g et emtpy
        X = false;
      }
      if (game_get_square(g0, i, j) !=
          S_EMPTY) {  // testé si tous les cases de g0 et emtpy
        Y = false;
      }
    }
  }
  if (A && B && (C == 8) && (D == 8) && (!G) && (!H) && (E == 6) && (F == 8) &&
      (X) && (Y)) {

    */
  return true;
}

bool test_game_is_wrapping(void) {
  game g = game_new_empty_ext(8, 8, true, true);  // wrapping option succes
  game g1 = game_default();                       // wrapping option no succes
  if (game_is_wrapping(g) && !game_is_wrapping(g1)) {
    return true;
  }
  return false;
}
/*****************/

bool test_game_load(void) {
  /**game g2 = game_new_empty_ext(8, 8, false, false);
  game_save(g2, "text2.txt");
  game g3 = game_load("text2.txt");
  game g = game_load("default.txt");
  game g1 = game_default();
  uint nb_rows = game_nb_rows(g1);
  uint nb_cols = game_nb_cols(g1);
  bool wrapping = game_is_wrapping(g);
  bool unique = game_is_unique(g);
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (game_get_square(g, i, j) != S_EMPTY && wrapping != false &&
          unique != false) {
        return false;
      }
    }
  }
  for (int i = 0; i < nb_rows; i++) {
    for (int j = 0; j < nb_cols; j++) {
      if (game_get_square(g, i, j) != game_get_square(g1, i, j) &&
          wrapping != false && unique != false) {
        return false;
      }
    }
  }

  game_delete(g);
  game_delete(g1);
  game_delete(g2);
  game_delete(g3);
**/
  return true;
}
bool test_game_undo(void) { return true; }
bool test_game_redo(void) { return true; }

bool test_game_save(void) {
  game g1 = game_default_solution();
  game_save(g1, "text4.txt");
  game g2 = game_load("text4.txt");
  uint nb_rows = game_nb_rows(g2);
  uint nb_cols = game_nb_cols(g2);
  bool wrapping = game_is_wrapping(g2);
  bool unique = game_is_unique(g2);
  for (int i = 0; i < nb_rows; i++) {
    for (int j = 0; j < nb_cols; j++) {
      if (game_get_square(g2, i, j) != game_get_square(g1, i, j) &&
          wrapping != false && unique != false) {
        return false;
      }
    }
  }
  game_delete(g1);
  game_delete(g2);
  return true;
}

/********************************************/

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("must include test names as arguments\n");
    return EXIT_FAILURE;
  }
  bool ok = false;
  if (strcmp("game_copy", argv[1]) == 0)
    ok = test_game_copy();
  else if (strcmp("game_equal", argv[1]) == 0)
    ok = test_game_equal();
  else if (strcmp("game_get_square", argv[1]) == 0)
    ok = test_game_get_square();
  else if (strcmp("game_get_next_number", argv[1]) == 0)
    ok = test_game_get_next_number();
  else if (strcmp("game_has_error", argv[1]) == 0)
    ok = test_game_has_error();
  else if (strcmp("game_is_over", argv[1]) == 0)
    ok = test_game_is_over();
  else if (strcmp("game_default", argv[1]) == 0)
    ok = test_game_default();
  else if (strcmp("game_new_empty_ext", argv[1]) == 0) {
    ok = test_game_new_empty_ext();
  } else if (strcmp("game_is_wrapping", argv[1]) == 0) {
    ok = test_game_is_wrapping();
  } else if (strcmp("game_load", argv[1]) == 0) {
    ok = test_game_load();
  } else if (strcmp("game_save", argv[1]) == 0) {
    ok = test_game_save();
  } else if (strcmp("game_undo", argv[1]) == 0) {
    ok = test_game_undo();
  } else if (strcmp("game_redo", argv[1]) == 0) {
    ok = test_game_redo();
  } else {
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
/********************************/
