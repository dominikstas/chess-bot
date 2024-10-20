#include "chess_board.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Piece board[BOARD_SIZE][BOARD_SIZE];

// Global variables to track king positions
int white_king_row = 0, white_king_col = 4;
int black_king_row = 7, black_king_col = 4;

// Global variables to track castling
int white_king_moved = 0, black_king_moved = 0;
int white_kingside_rook_moved = 0, white_queenside_rook_moved = 0;
int black_kingside_rook_moved = 0, black_queenside_rook_moved = 0;

// Global variables to track en passant state
int en_passant_row = -1, en_passant_col = -1;

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

// Pawn

int last_pawn_double_move_col = -1; // Add this new global variable

int is_valid_pawn_move(int from_row, int from_col, int to_row, int to_col) {
  char piece = board[from_row][from_col];
  int direction = (piece == 'P') ? 1 : -1;
  int start_row = (piece == 'P') ? 1 : 6;

  // Regular move: one square forward
  if (from_col == to_col && to_row == from_row + direction &&
      board[to_row][to_col] == ' ') {
    return 1;
  }

  // Two squares forward from starting position
  if (from_col == to_col && from_row == start_row &&
      to_row == from_row + 2 * direction &&
      board[from_row + direction][from_col] == ' ' &&
      board[to_row][to_col] == ' ') {
    en_passant_row = from_row + direction; // Changed this line
    en_passant_col = from_col;             // Changed this line
    last_pawn_double_move_col =
        from_col; // Track the column of the pawn that moved
    return 1;
  }

  // Capture diagonally
  if (abs(to_col - from_col) == 1 && to_row == from_row + direction) {
    char target = board[to_row][to_col];
    // Normal capture
    if (target != ' ' && ((piece == 'P' && target >= 'a' && target <= 'z') ||
                          (piece == 'p' && target >= 'A' && target <= 'Z'))) {
      return 1;
    }

    // En passant capture
    if (to_row == en_passant_row && to_col == last_pawn_double_move_col) {
      // The captured pawn is on the same row as the capturing pawn
      if (board[from_row][to_col] == (piece == 'P' ? 'p' : 'P')) {
        board[from_row][to_col] = ' '; // Remove the captured pawn
        return 1;
      }
    }
  }

  return 0;
}

// Add this function to reset en passant state after each move
void reset_en_passant() {
  en_passant_row = -1;
  en_passant_col = -1;
  last_pawn_double_move_col = -1;
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

int can_castle(int from_row, int from_col, int to_row, int to_col,
               int current_player) {
  // Only allow castling if the king is moving exactly two squares horizontally
  if (abs(to_col - from_col) != 2 || from_row != to_row) {
    return 0;
  }

  // Get the correct row based on player
  int castle_row = (current_player == 0) ? 0 : 7;

  // Verify we're actually moving from the correct starting position
  if (from_row != castle_row || from_col != 4) {
    return 0;
  }

  // Check if the king or corresponding rook has moved
  if (current_player == 0) {
    if (white_king_moved)
      return 0;
    if (to_col > from_col && white_kingside_rook_moved)
      return 0; // Kingside
    if (to_col < from_col && white_queenside_rook_moved)
      return 0; // Queenside
  } else {
    if (black_king_moved)
      return 0;
    if (to_col > from_col && black_kingside_rook_moved)
      return 0; // Kingside
    if (to_col < from_col && black_queenside_rook_moved)
      return 0; // Queenside
  }

  // Check if the squares between king and rook are empty
  int rook_col = (to_col > from_col) ? 7 : 0;
  int step = (to_col > from_col) ? 1 : -1;
  for (int col = from_col + step; col != rook_col; col += step) {
    if (board[from_row][col] != ' ') {
      return 0;
    }
  }

  // Verify rook is present
  char expected_rook = (current_player == 0) ? 'R' : 'r';
  if (board[from_row][rook_col] != expected_rook) {
    return 0;
  }

  // Check if king is in check or would pass through check
  for (int col = from_col; col != to_col + step; col += step) {
    if (is_in_check(from_row, col, current_player)) {
      return 0;
    }
  }

  return 1;
}

int is_valid_king_move(int from_row, int from_col, int to_row, int to_col,
                       int current_player) {
  int row_diff = abs(to_row - from_row);
  int col_diff = abs(to_col - from_col);

  // Check for castling
  if (row_diff == 0 && col_diff == 2) {
    return can_castle(from_row, from_col, to_row, to_col, current_player);
  }

  // Regular king move
  if (row_diff > 1 || col_diff > 1) {
    return 0;
  }

  char target_piece = board[to_row][to_col];
  if (target_piece != ' ') {
    if ((board[from_row][from_col] >= 'A' && board[from_row][from_col] <= 'Z' &&
         target_piece >= 'A' && target_piece <= 'Z') ||
        (board[from_row][from_col] >= 'a' && board[from_row][from_col] <= 'z' &&
         target_piece >= 'a' && target_piece <= 'z')) {
      return 0;
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

  switch (toupper(piece)) {
  case 'P':
    return is_valid_pawn_move(from_row, from_col, to_row, to_col);
  case 'R':
    return is_valid_rook_move(from_row, from_col, to_row, to_col);
  case 'B':
    return is_valid_bishop_move(from_row, from_col, to_row, to_col);
  case 'N':
    return is_valid_knight_move(from_row, from_col, to_row, to_col);
  case 'Q':
    return is_valid_queen_move(from_row, from_col, to_row, to_col);
  case 'K':
    return is_valid_king_move(from_row, from_col, to_row, to_col,
                              current_player);
  default:
    printf("Unrecognized piece\n");
    return 0;
  }
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

  // Handle castling
  int is_castling = 0;
  int rook_from_col = -1;
  int rook_to_col = -1;

  if ((piece == 'K' || piece == 'k') && abs(to_col - from_col) == 2) {
    is_castling = 1;
    // Determine rook positions
    if (to_col > from_col) { // Kingside
      rook_from_col = 7;
      rook_to_col = from_col + 1;
    } else { // Queenside
      rook_from_col = 0;
      rook_to_col = from_col - 1;
    }
  }

  // Make the move
  board[to_row][to_col] = board[from_row][from_col];
  board[from_row][from_col] = ' ';

  // Complete castling by moving the rook
  if (is_castling) {
    board[to_row][rook_to_col] = board[from_row][rook_from_col];
    board[from_row][rook_from_col] = ' ';
  }

  // Update king position if the king moved
  if (piece == 'K' || piece == 'k') {
    if (current_player == 0) {
      white_king_row = to_row;
      white_king_col = to_col;
      white_king_moved = 1;
    } else {
      black_king_row = to_row;
      black_king_col = to_col;
      black_king_moved = 1;
    }
  }

  // Update rook movement flags
  if (piece == 'R' || piece == 'r') {
    if (from_row == 0 && from_col == 0)
      white_queenside_rook_moved = 1;
    if (from_row == 0 && from_col == 7)
      white_kingside_rook_moved = 1;
    if (from_row == 7 && from_col == 0)
      black_queenside_rook_moved = 1;
    if (from_row == 7 && from_col == 7)
      black_kingside_rook_moved = 1;
  }

  // Check if own king is in check after the move
  if (is_in_check((current_player == 0) ? white_king_row : black_king_row,
                  (current_player == 0) ? white_king_col : black_king_col,
                  current_player)) {
    // Undo the move
    board[from_row][from_col] = piece;
    board[to_row][to_col] = original_target;
    if (is_castling) {
      // Undo rook move
      board[from_row][rook_from_col] = board[to_row][rook_to_col];
      board[to_row][rook_to_col] = ' ';
    }
    // Reset king position if it was a king move
    if (piece == 'K' || piece == 'k') {
      if (current_player == 0) {
        white_king_row = from_row;
        white_king_col = from_col;
        white_king_moved = 0;
      } else {
        black_king_row = from_row;
        black_king_col = from_col;
        black_king_moved = 0;
      }
    }
    return 0;
  }

  // Reset en passant state after a successful move
  if (toupper(piece) != 'P' || abs(to_row - from_row) != 2) {
    reset_en_passant();
  }

  // Check for check/checkmate (rest of the original function remains the same)
  int opponent_king_row =
      (current_player == 0) ? black_king_row : white_king_row;
  int opponent_king_col =
      (current_player == 0) ? black_king_col : white_king_col;

  if (is_in_check(opponent_king_row, opponent_king_col, 1 - current_player)) {
    if (is_checkmate(opponent_king_row, opponent_king_col,
                     1 - current_player)) {
      printf("Checkmate! Player %d wins!\n", current_player + 1);
      exit(0);
    } else {
      printf("Check for Player %d!\n", 1 - current_player + 1);
    }
  }

  return 1;
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