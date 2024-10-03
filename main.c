#include "chess_board.h"
#include <stdio.h>
#include <string.h>

#define MAX_MOVE_LENGTH 5

int main() {
  initialize_board();
  printf("Welcome to the chess game!\n");
  print_board();

  char move_str[MAX_MOVE_LENGTH];
  int from_row, from_col, to_row, to_col;
  int current_player = 0; // 0 for White, 1 for Black

  while (1) {
    printf("\n%s player's turn. Enter your move (e.g., e2e4): ",
           current_player == 0 ? "White" : "Black");

    // Clear input buffer before reading new input
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
      ;

    if (fgets(move_str, sizeof(move_str), stdin) == NULL) {
      printf("Error reading input. Exiting...\n");
      break;
    }

    // Remove newline character if present
    size_t len = strlen(move_str);
    if (len > 0 && move_str[len - 1] == '\n') {
      move_str[len - 1] = '\0';
    }

    // Ignore empty moves
    if (strlen(move_str) == 0) {
      continue;
    }

    // Check for quit command
    if (strcmp(move_str, "quit") == 0) {
      printf("Game ended. Thanks for playing!\n");
      break;
    }

    if (parse_move(move_str, &from_row, &from_col, &to_row, &to_col)) {
      if (move_piece(from_row, from_col, to_row, to_col, current_player)) {

        print_board();
        current_player = 1 - current_player; // Switch players
      } else {
        printf("Invalid move. Try again.\n");
      }
    } else {
      printf("Invalid input format. Please use the format 'e2e4'.\n");
    }
  }

  return 0;
}