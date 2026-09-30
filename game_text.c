#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_private.h"
#include "game_tools.h"

int main(int argc, char* argv[]) {
  game g;
  g = game_default();
  char c;
  int i, j;
  bool exit = false;
  while (game_is_over(g) != true && exit == false) {
    game_print(g);
    for (i = 0; i <= 5; i++) {
      for (j = 0; j <= 5; j++) {
        if (game_has_error(g, i, j) != 0) {
          printf("You've got some errors on (%d, %d)", i, j);
        }
      }
    }
    printf("Enter a character : ");
    if (scanf(" %c", &c) != 1) {
      printf("Error reading input character\n");
      return (EXIT_FAILURE);
    }

    if (c == 'h') {
      printf("S.O.S !");
      printf("-press 'w <i> <j>' to put a zero/white at square (i, j)");
      printf("-press 'b <i> <j>' to put a one/black at square (i, j)");
      printf("-press 'e <i> <j>' to empty square (i, j)");
      printf("-press 'r' to restart");
      printf("-press 'q' to quit");

    } else if (c == 'r') {
      printf("Reset the game !");
      game_restart(g);
    } else if (c == 'q') {
      exit = false;
      printf("What a shame, you lost !");
      return EXIT_SUCCESS;
    } else if (c == 'z') {
      printf("> undo\n");
      game_undo(g);
    } else if (c == 'y') {
      printf("> redo\n");
      game_redo(g);
    } else if ((c == 'w') || (c == 'b') || (c = 'e')) {
      printf("Enter the values of i and j : ");
      if (scanf("%d %d", &i, &j) != 2) {
        printf("Error: invalid input\n");
        continue;
      }

      if (game_check_move(g, i, j, S_ONE)) {
        if (c == 'w') {
          game_play_move(g, i, j, S_ZERO);
        } else if (c == 'b') {
          game_play_move(g, i, j, S_ONE);
        } else {
          game_play_move(g, i, j, S_EMPTY);
        }
      }
    }

    else if (c == 's') {
      char file_path[100];
      int ret = scanf(" %s", file_path);
      if (ret != 1) {
        printf("Error reading file path\n");
        continue;
      }
      printf("action: save file to %s\n", file_path);
      game_save(g, file_path);
    } else {
      printf("Non valid argument.\n");
    }
  }
  if (game_is_over(g) != false) {
    printf("Congratulation, you won !");
  }

  game_print(g);
  game_delete(g);
  return EXIT_SUCCESS;
}
