#include "chess_board.h"
#include <stdio.h>
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
      board[to_row][to_col] == ' ') { // Use ' ' for empty square
    printf("Valid one square forward move\n");
    return 1;
  }

  // Move two squares forward (only from the starting position)
  if (from_col == to_col && from_row == start_row &&
      to_row == from_row + 2 * direction &&
      board[from_row + direction][from_col] == ' ' &&
      board[to_row][to_col] == ' ') {
    printf("Valid two square forward move\n");
    return 1;
  }

  // capture diagonally
  if ((to_col == from_col - 1 || to_col == from_col + 1) &&
      to_row == from_row + direction) {
    Piece target = board[to_row][to_col];
    if (target != EMPTY && ((piece == WHITE_PAWN && target >= 'a' &&
                             target <= 'z') || // black piece
                            (piece == BLACK_PAWN && target >= 'A' &&
                             target <= 'Z'))) { // white piece
      return 1;
    }
  }

  // TO DO: en passant
  printf("Invalid pawn move\n");
  return 0;
}

// TO DO: ROOK
// TO DO: KNIGHT
// TO DO: BISHOP
// TO DO: QUEEN
// TO DO: KING

int is_valid_move(int from_row, int from_col, int to_row, int to_col) {
  char piece = board[from_row][from_col];

  if (piece == ' ') {
    return 0; // no piece to move
  }

  switch (piece) {
  case 'P':
  case 'p':
    return is_valid_pawn_move(from_row, from_col, to_row, to_col);
  // TODO: Implement validation for the rest
  default:
    return 0; // Invalid if it's an unrecognized piece
  }
}

int move_piece(int from_row, int from_col, int to_row, int to_col) {
  if (!is_valid_move(from_row, from_col, to_row, to_col)) {
    return 0; // invalid
  }

  board[to_row][to_col] = board[from_row][from_col];
  board[from_row][from_col] = EMPTY;

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
