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
    return 1;
  }

  // Move two squares forward (only from the starting position)
  if (from_col == to_col && from_row == start_row &&
      to_row == from_row + 2 * direction &&
      board[from_row + direction][from_col] == ' ' &&
      board[to_row][to_col] == ' ') {
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

int is_valid_rook_move(int from_row, int from_col, int to_row, int to_col) {
  // Sprawdź, czy ruch jest w tej samej linii (poziomo lub pionowo)
  if (from_row != to_row && from_col != to_col) {
    printf("Wieża może poruszać się tylko poziomo lub pionowo\n");
    return 0;
  }

  // Określ kierunek ruchu
  int row_direction = (to_row > from_row) ? 1 : ((to_row < from_row) ? -1 : 0);
  int col_direction = (to_col > from_col) ? 1 : ((to_col < from_col) ? -1 : 0);

  // Sprawdź, czy na drodze nie ma innych figur
  int current_row = from_row + row_direction;
  int current_col = from_col + col_direction;

  while (current_row != to_row || current_col != to_col) {
    if (board[current_row][current_col] != ' ') {
      printf("Na drodze wieży znajduje się inna figura\n");
      return 0;
    }
    current_row += row_direction;
    current_col += col_direction;
  }

  // Sprawdź, czy na polu docelowym nie ma figury tego samego koloru
  char moving_piece = board[from_row][from_col];
  char target_piece = board[to_row][to_col];

  if (target_piece != ' ') {
    if ((moving_piece >= 'A' && moving_piece <= 'Z' && target_piece >= 'A' &&
         target_piece <= 'Z') ||
        (moving_piece >= 'a' && moving_piece <= 'z' && target_piece >= 'a' &&
         target_piece <= 'z')) {
      printf("Nie można zbić własnej figury\n");
      return 0;
    }
  }

  return 1;
}
// TO DO: ROOK

// TO DO: KNIGHT
// TO DO: BISHOP
// TO DO: QUEEN
// TO DO: KING

int is_valid_move(int from_row, int from_col, int to_row, int to_col,
                  int current_player) {
  char piece = board[from_row][from_col];

  if (piece == ' ') {
    printf("No piece to move\n");
    return 0; // no piece to move
  }

  // Check if the player is moving their own piece
  if (current_player == 0) { // White player
    if (piece >= 'a' && piece <= 'z') {
      printf("White player cannot move black pieces\n");
      return 0; // White player trying to move black piece
    }
  } else { // Black player
    if (piece >= 'A' && piece <= 'Z') {
      printf("Black player cannot move white pieces\n");
      return 0; // Black player trying to move white piece
    }
  }

  switch (piece) {
  case 'P':
  case 'p':
    return is_valid_pawn_move(from_row, from_col, to_row, to_col);
  case 'r':
  case 'R':
    return is_valid_rook_move(from_row, from_col, to_row, to_col);
    // TODO: Implement validation for the rest
  default:
    printf("Unrecognized piece\n");
    return 0; // Invalid if it's an unrecognized piece
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
