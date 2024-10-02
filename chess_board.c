#include "chess_board.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Piece board[BOARD_SIZE][BOARD_SIZE];

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
  printf("Invalid pawn move\n");
  return 0;
}

// Rook
int is_valid_rook_move(int from_row, int from_col, int to_row, int to_col) {
  if (from_row != to_row && from_col != to_col) {
    printf("Invalid rook move\n");
    return 0;
  }
  int row_direction = (to_row > from_row) ? 1 : ((to_row < from_row) ? -1 : 0);
  int col_direction = (to_col > from_col) ? 1 : ((to_col < from_col) ? -1 : 0);
  int current_row = from_row + row_direction;
  int current_col = from_col + col_direction;

  while (current_row != to_row || current_col != to_col) {
    if (board[current_row][current_col] != ' ') {
      printf("Invalid rook move\n");
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
      printf("Invalid rook move\n");
      return 0;
    }
  }

  return 1;
}

// bishop
int is_valid_bishop_move(int from_row, int from_col, int to_row, int to_col) {
  char piece = board[from_row][from_col];

  if (abs(to_row - from_row) != abs(to_col - from_col)) {
    printf("Invalid bishop move\n");
    return 0;
  }
  int row_direction = (to_row > from_row) ? 1 : -1;
  int col_direction = (to_col > from_col) ? 1 : -1;

  int current_row = from_row + row_direction;
  int current_col = from_col + col_direction;

  while (current_row != to_row && current_col != to_col) {
    if (board[current_row][current_col] != ' ') {
      printf("Invalid bishop move\n");
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
      printf("Invalid bishop move\n");
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
      printf("Invalid knight move: cannot capture own piece\n");
      return 0;
    }
  }

  // Knights move in an L-shape: 2 squares in one direction and 1 square
  // perpendicular
  int row_diff = abs(to_row - from_row);
  int col_diff = abs(to_col - from_col);

  if ((row_diff == 2 && col_diff == 1) || (row_diff == 1 && col_diff == 2)) {
    return 1;
  }

  printf("Invalid knight move: must move in L-shape\n");
  return 0;
}

// TO DO: QUEEN
// TO DO: KING

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
  // TODO: Implement validation for the rest
  default:
    printf("Unrecognized piece\n");
    return 0;
  }
}

int move_piece(int from_row, int from_col, int to_row, int to_col,
               int current_player) {
  if (!is_valid_move(from_row, from_col, to_row, to_col, current_player)) {
    return 0; // invalid
  }

  board[to_row][to_col] = board[from_row][from_col];
  board[from_row][from_col] = ' ';

  return 1; // valid
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
