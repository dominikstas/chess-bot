#include "chess_board.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Piece board[BOARD_SIZE][BOARD_SIZE];

// Global variables to track king positions
int white_king_row = 0, white_king_col = 4;
int black_king_row = 7, black_king_col = 4;

void initialize_board() {
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      board[i][j] = ' ';
    }
  }
  // white - big leters
  board[0][0] = board[0][7] = 'R'; // Rook
  board[0][1] = board[0][6] = 'N'; // Knight
  board[0][2] = board[0][5] = 'B'; // Bishop
  board[0][3] = 'Q';               // Queen
  board[0][4] = 'K';               // King
  for (int i = 0; i < BOARD_SIZE; i++) {
    board[1][i] = 'P'; // Pawns
  }

  // black - small letters
  board[7][0] = board[7][7] = 'r'; // Rook
  board[7][1] = board[7][6] = 'n'; // Knight
  board[7][2] = board[7][5] = 'b'; // Bishop
  board[7][3] = 'q';               // Queen
  board[7][4] = 'k';               // King
  for (int i = 0; i < BOARD_SIZE; i++) {
    board[6][i] = 'p'; // Pawns
  }
}

void print_board() {
  printf("     a   b   c   d   e   f   g   h\n");
  printf("   +---+---+---+---+---+---+---+---+\n");
  for (int i = BOARD_SIZE - 1; i >= 0; i--) {
    printf(" %d |", i + 1);
    for (int j = 0; j < BOARD_SIZE; j++) {
      printf(" %c |", board[i][j]);
    }
    printf(" %d\n", i + 1);
    printf("   +---+---+---+---+---+---+---+---+\n");
  }
  printf("     a   b   c   d   e   f   g   h\n");
}

// check the move for all pieces

// Pawn (WITHOUT EN PASSANT)
int is_valid_pawn_move(int from_row, int from_col, int to_row, int to_col) {
  char piece = board[from_row][from_col];
  int direction = (piece == 'P') ? 1 : -1; // 'P' for white pawn, moving up
  int start_row = (piece == 'P') ? 1 : 6;

  // Move one square forward
  if (from_col == to_col && to_row == from_row + direction &&
      board[to_row][to_col] == ' ') {
    return 1;
  }

  // Move two squares forward (only from the starting position)
  if (from_col == to_col && from_row == start_row &&
      to_row == from_row + 2 * direction &&
      board[from_row + direction][from_col] == ' ' &&
      board[to_row][to_col] == ' ') {
    return 1;
  }

  // Capture diagonally
  if ((to_col == from_col - 1 || to_col == from_col + 1) &&
      to_row == from_row + direction) {
    char target = board[to_row][to_col];
    if (target != ' ' && ((piece == 'P' && target >= 'a' &&
                           target <= 'z') || // white pawn captures black piece
                          (piece == 'p' && target >= 'A' &&
                           target <= 'Z'))) { // black pawn captures white piece
      return 1;
    }
  }

  // TO DO: en passant
  return 0;
}

// Rook
int is_valid_rook_move(int from_row, int from_col, int to_row, int to_col) {
  if (from_row != to_row && from_col != to_col) {
    return 0;
  }
  int row_direction = (to_row > from_row) ? 1 : ((to_row < from_row) ? -1 : 0);
  int col_direction = (to_col > from_col) ? 1 : ((to_col < from_col) ? -1 : 0);
  int current_row = from_row + row_direction;
  int current_col = from_col + col_direction;

  while (current_row != to_row || current_col != to_col) {
    if (board[current_row][current_col] != ' ') {
      return 0;
    }
    current_row += row_direction;
    current_col += col_direction;
  }

  char moving_piece = board[from_row][from_col];
  char target_piece = board[to_row][to_col];

  if (target_piece != ' ') {
    if ((moving_piece >= 'A' && moving_piece <= 'Z' && target_piece >= 'A' &&
         target_piece <= 'Z') ||
        (moving_piece >= 'a' && moving_piece <= 'z' && target_piece >= 'a' &&
         target_piece <= 'z')) {
      return 0;
    }
  }

  return 1;
}

// bishop
int is_valid_bishop_move(int from_row, int from_col, int to_row, int to_col) {
  char piece = board[from_row][from_col];

  if (abs(to_row - from_row) != abs(to_col - from_col)) {
    return 0;
  }
  int row_direction = (to_row > from_row) ? 1 : -1;
  int col_direction = (to_col > from_col) ? 1 : -1;

  int current_row = from_row + row_direction;
  int current_col = from_col + col_direction;

  while (current_row != to_row && current_col != to_col) {
    if (board[current_row][current_col] != ' ') {
      return 0;
    }
    current_row += row_direction;
    current_col += col_direction;
  }

  char target_piece = board[to_row][to_col];
  if (target_piece != ' ') {
    if ((piece >= 'A' && piece <= 'Z' && target_piece >= 'A' &&
         target_piece <= 'Z') ||
        (piece >= 'a' && piece <= 'z' && target_piece >= 'a' &&
         target_piece <= 'z')) {
      return 0;
    }
  }

  return 1;
}

// Knight
int is_valid_knight_move(int from_row, int from_col, int to_row, int to_col) {
  char piece = board[from_row][from_col];
  char target_piece = board[to_row][to_col];

  // Check if the target square has a piece of the same color
  if (target_piece != ' ') {
    if ((piece >= 'A' && piece <= 'Z' && target_piece >= 'A' &&
         target_piece <= 'Z') ||
        (piece >= 'a' && piece <= 'z' && target_piece >= 'a' &&
         target_piece <= 'z')) {
      return 0;
    }
  }

  // Knights move: 2 squares in one direction and 1 square perpendicular
  int row_diff = abs(to_row - from_row);
  int col_diff = abs(to_col - from_col);

  if ((row_diff == 2 && col_diff == 1) || (row_diff == 1 && col_diff == 2)) {
    return 1;
  }

  return 0;
}

int is_valid_queen_move(int from_row, int from_col, int to_row, int to_col) {

  // First, check if it's a valid rook move
  if (from_row == to_row || from_col == to_col) {
    return is_valid_rook_move(from_row, from_col, to_row, to_col);
  }
  // If not a rook move, check if it's a valid bishop move
  else if (abs(to_row - from_row) == abs(to_col - from_col)) {
    return is_valid_bishop_move(from_row, from_col, to_row, to_col);
  }

  return 0;
}

// King movement validation
int is_valid_king_move(int from_row, int from_col, int to_row, int to_col) {
  // Regular king move: one square in any direction
  int row_diff = abs(to_row - from_row);
  int col_diff = abs(to_col - from_col);

  if (row_diff > 1 || col_diff > 1) {
    return 0;
  }

  // Check if the target square has a piece of the same color
  char piece = board[from_row][from_col];
  char target_piece = board[to_row][to_col];
  if (target_piece != ' ') {
    if ((piece >= 'A' && piece <= 'Z' && target_piece >= 'A' &&
         target_piece <= 'Z') ||
        (piece >= 'a' && piece <= 'z' && target_piece >= 'a' &&
         target_piece <= 'z')) {
      return 0;
    }
  }

  // TODO: Add castling logic here if desired

  return 1;
}

int is_in_check(int king_row, int king_col, int current_player) {
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      char piece = board[i][j];
      if ((current_player == 0 && piece >= 'a' &&
           piece <= 'z') || // Check black pieces
          (current_player == 1 && piece >= 'A' &&
           piece <= 'Z')) { // Check white pieces
        if (is_valid_move(i, j, king_row, king_col,
                          (current_player == 0 ? 1 : 0))) {
          return 1; // King is in check
        }
      }
    }
  }
  return 0; // King is not in check
}

int is_checkmate(int king_row, int king_col, int current_player) {
  // If the king is not in check, it's definitely not checkmate
  if (!is_in_check(king_row, king_col, current_player)) {
    return 0;
  }

  // Check all possible moves for all pieces of the player
  for (int from_row = 0; from_row < BOARD_SIZE; from_row++) {
    for (int from_col = 0; from_col < BOARD_SIZE; from_col++) {
      char piece = board[from_row][from_col];

      // Check only the current player's pieces
      if ((current_player == 0 && (piece < 'A' || piece > 'Z')) ||
          (current_player == 1 && (piece < 'a' || piece > 'z'))) {
        continue;
      }

      // Check all possible destination squares
      for (int to_row = 0; to_row < BOARD_SIZE; to_row++) {
        for (int to_col = 0; to_col < BOARD_SIZE; to_col++) {
          // Check if the move is legal
          if (is_valid_move(from_row, from_col, to_row, to_col,
                            current_player)) {
            // Make the move temporarily
            char temp = board[to_row][to_col];
            board[to_row][to_col] = board[from_row][from_col];
            board[from_row][from_col] = ' ';

            // Update king position if the king moved
            int temp_king_row = king_row;
            int temp_king_col = king_col;
            if ((piece == 'K' || piece == 'k') &&
                (from_row == king_row && from_col == king_col)) {
              temp_king_row = to_row;
              temp_king_col = to_col;
            }

            // Check if the king is still in check after this move
            int still_in_check =
                is_in_check(temp_king_row, temp_king_col, current_player);

            // Undo the move
            board[from_row][from_col] = board[to_row][to_col];
            board[to_row][to_col] = temp;

            // If we found a move that removes the check, it's not checkmate
            if (!still_in_check) {
              return 0;
            }
          }
        }
      }
    }
  }

  // If we got here, it means we couldn't find any move
  // that would remove the check - it's checkmate
  return 1;
}

int move_piece(int from_row, int from_col, int to_row, int to_col,
               int current_player) {
  char piece = board[from_row][from_col];

  // Check if the move is legal
  if (!is_valid_move(from_row, from_col, to_row, to_col, current_player)) {
    return 0;
  }

  // Save positions before making the move
  char original_target = board[to_row][to_col];
  int king_row = (current_player == 0) ? white_king_row : black_king_row;
  int king_col = (current_player == 0) ? white_king_col : black_king_col;

  // Make the move
  board[to_row][to_col] = board[from_row][from_col];
  board[from_row][from_col] = ' ';

  // Update king position if the king moved
  if (piece == 'K' || piece == 'k') {
    if (current_player == 0) {
      white_king_row = to_row;
      white_king_col = to_col;
    } else {
      black_king_row = to_row;
      black_king_col = to_col;
    }
  }

  // Check if own king is in check after the move
  if (is_in_check(king_row, king_col, current_player)) {
    // Undo the move
    board[from_row][from_col] = board[to_row][to_col];
    board[to_row][to_col] = original_target;
    return 0;
  }

  // Check if opponent is in check
  int opponent_king_row =
      (current_player == 0) ? black_king_row : white_king_row;
  int opponent_king_col =
      (current_player == 0) ? black_king_col : white_king_col;

  if (is_in_check(opponent_king_row, opponent_king_col, 1 - current_player)) {
    if (is_checkmate(opponent_king_row, opponent_king_col,
                     1 - current_player)) {
      printf("Checkmate! Player %d wins!\n", current_player + 1);
      exit(0); // End the game
    } else {
      printf("Check for Player %d!\n", 1 - current_player + 1);
    }
  }

  return 1;
}

int is_valid_move(int from_row, int from_col, int to_row, int to_col,
                  int current_player) {
  char piece = board[from_row][from_col];

  if (piece == ' ') {
    printf("No piece to move\n");
    return 0;
  }

  // Check if the player is moving their own piece
  if (current_player == 0) { // White player
    if (piece >= 'a' && piece <= 'z') {
      printf("White player cannot move black pieces\n");
      return 0;
    }
  } else { // Black player
    if (piece >= 'A' && piece <= 'Z') {
      printf("Black player cannot move white pieces\n");
      return 0;
    }
  }

  switch (piece) {
  case 'P':
  case 'p':
    return is_valid_pawn_move(from_row, from_col, to_row, to_col);
  case 'R':
  case 'r':
    return is_valid_rook_move(from_row, from_col, to_row, to_col);
  case 'B':
  case 'b':
    return is_valid_bishop_move(from_row, from_col, to_row, to_col);
  case 'N':
  case 'n':
    return is_valid_knight_move(from_row, from_col, to_row, to_col);
  case 'Q':
  case 'q':
    return is_valid_queen_move(from_row, from_col, to_row, to_col);
  case 'K':
  case 'k':
    return is_valid_king_move(from_row, from_col, to_row, to_col);
  default:
    printf("Unrecognized piece\n");
    return 0;
  }
}

int parse_move(const char *move_str, int *from_row, int *from_col, int *to_row,
               int *to_col) {
  if (strlen(move_str) != 4) {
    return 0; // Invalid input length
  }

  *from_col = move_str[0] - 'a'; // Convert column 'a'-'h' to 0-7
  *from_row = move_str[1] - '1'; // Convert row '1'-'8' to 0-7
  *to_col = move_str[2] - 'a';   // Convert column 'a'-'h' to 0-7
  *to_row = move_str[3] - '1';   // Convert row '1'-'8' to 0-7

  // Check if the parsed values are within the valid board range
  if (*from_col < 0 || *from_col >= BOARD_SIZE || *from_row < 0 ||
      *from_row >= BOARD_SIZE || *to_col < 0 || *to_col >= BOARD_SIZE ||
      *to_row < 0 || *to_row >= BOARD_SIZE) {
    return 0; // Invalid move
  }

  return 1; // Valid move input
}