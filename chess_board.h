#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#define BOARD_SIZE 8

typedef enum {
  EMPTY,
  WHITE_PAWN,
  WHITE_ROOK,
  WHITE_KNIGHT,
  WHITE_BISHOP,
  WHITE_QUEEN,
  WHITE_KING,
  BLACK_PAWN,
  BLACK_ROOK,
  BLACK_KNIGHT,
  BLACK_BISHOP,
  BLACK_QUEEN,
  BLACK_KING,
} Piece;

void initialize_board();
void print_board();
int move_piece(int from_row, int from_col, int to_row, int to_col);

#endif