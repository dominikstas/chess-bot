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

  // Knights move: 2 squares in one direction and 1 square perpendicular
  int row_diff = abs(to_row - from_row);
  int col_diff = abs(to_col - from_col);

  if ((row_diff == 2 && col_diff == 1) || (row_diff == 1 && col_diff == 2)) {
    return 1;
  }

  printf("Invalid knight move\n");
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

  printf("Invalid queen move\n");
  return 0;
}

// King movement validation
int is_valid_king_move(int from_row, int from_col, int to_row, int to_col) {
  // Regular king move: one square in any direction
  int row_diff = abs(to_row - from_row);
  int col_diff = abs(to_col - from_col);

  if (row_diff > 1 || col_diff > 1) {
    printf("Invalid king move: can only move one square in any direction\n");
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
      printf("Invalid king move: cannot capture own piece\n");
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
           piece <= 'z') || // Sprawdzenie czarnych figur
          (current_player == 1 && piece >= 'A' &&
           piece <= 'Z')) { // Sprawdzenie białych figur
        if (is_valid_move(i, j, king_row, king_col,
                          (current_player == 0 ? 1 : 0))) {
          return 1; // Król jest w szachu
        }
      }
    }
  }
  return 0; // Król nie jest w szachu
}

int is_checkmate(int king_row, int king_col, int current_player) {
  if (!is_in_check(king_row, king_col, current_player)) {
    return 0; // Król nie jest w szachu, więc nie ma mata
  }

  // Sprawdzamy wszystkie możliwe ruchy, aby zobaczyć, czy można uciec z szacha
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      char piece = board[i][j];
      if ((current_player == 0 && piece >= 'A' &&
           piece <= 'Z') || // Białe figury
          (current_player == 1 && piece >= 'a' &&
           piece <= 'z')) { // Czarne figury
        for (int to_row = 0; to_row < BOARD_SIZE; to_row++) {
          for (int to_col = 0; to_col < BOARD_SIZE; to_col++) {
            if (is_valid_move(i, j, to_row, to_col, current_player)) {
              // Symulacja ruchu
              char temp = board[to_row][to_col];
              board[to_row][to_col] = board[i][j];
              board[i][j] = ' ';

              // Sprawdzenie, czy król wciąż jest w szachu po ruchu
              if (!is_in_check(king_row, king_col, current_player)) {
                // Cofamy ruch
                board[i][j] = board[to_row][to_col];
                board[to_row][to_col] = temp;
                return 0; // Można uniknąć mata
              }

              // Cofamy ruch
              board[i][j] = board[to_row][to_col];
              board[to_row][to_col] = temp;
            }
          }
        }
      }
    }
  }

  return 1; // Mat
}

int move_piece(int from_row, int from_col, int to_row, int to_col,
               int current_player) {
  char piece = board[from_row][from_col];

  // Sprawdzenie, czy ruch jest legalny
  if (!is_valid_move(from_row, from_col, to_row, to_col, current_player)) {
    return 0;
  }

  // Zapisujemy pozycje przed wykonaniem ruchu
  char original_target = board[to_row][to_col];
  int king_row = (current_player == 0) ? white_king_row : black_king_row;
  int king_col = (current_player == 0) ? white_king_col : black_king_col;

  // Wykonanie ruchu
  board[to_row][to_col] = board[from_row][from_col];
  board[from_row][from_col] = ' ';

  // Aktualizacja pozycji króla, jeśli to król się poruszył
  if (piece == 'K' || piece == 'k') {
    if (current_player == 0) {
      white_king_row = to_row;
      white_king_col = to_col;
    } else {
      black_king_row = to_row;
      black_king_col = to_col;
    }
  }

  // Sprawdzamy, czy własny król jest w szachu po ruchu
  if (is_in_check(king_row, king_col, current_player)) {
    // Cofamy ruch
    board[from_row][from_col] = board[to_row][to_col];
    board[to_row][to_col] = original_target;

    printf("Ruch nielegalny, ponieważ własny król jest szachowany\n");
    return 0;
  }

  // Sprawdzamy, czy przeciwnik jest w szachu
  int opponent_king_row =
      (current_player == 0) ? black_king_row : white_king_row;
  int opponent_king_col =
      (current_player == 0) ? black_king_col : white_king_col;

  if (is_in_check(opponent_king_row, opponent_king_col, 1 - current_player)) {
    if (is_checkmate(opponent_king_row, opponent_king_col,
                     1 - current_player)) {
      printf("Szach mat! Gracz %d wygrywa!\n", current_player + 1);
      exit(0); // Kończymy grę
    } else {
      printf("Szach dla gracza %d!\n", 1 - current_player + 1);
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
