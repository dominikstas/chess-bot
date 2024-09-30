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

    if (fgets(move_str, sizeof(move_str), stdin) == NULL) {
      printf("Error reading input. Exiting...\n");
      break;
    }

    // Remove newline character if present
    move_str[strcspn(move_str, "\n")] = 0;

    // Check for quit command
    if (strcmp(move_str, "quit") == 0) {
      printf("Game ended. Thanks for playing!\n");
      break;
    }

    if (parse_move(move_str, &from_row, &from_col, &to_row, &to_col)) {
      if (move_piece(from_row, from_col, to_row, to_col)) {
        print_board();
        current_player = 1 - current_player; // Switch players
      } else {
        printf("Invalid move. Try again.\n");
      }
    } else {
      printf("Invalid input format. Please use the format 'e2e4'.\n");
    }

    // Clear input buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
      ;
  }

  return 0;
}