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
int is_valid_pawn_move(int from_row, int from_col, int to_row, int to_col);
int is_valid_rook_move(int from_row, int from_col, int to_row, int to_col);
int is_valid_bishop_move(int from_row, int from_col, int to_row, int to_col);
int is_valid_knight_move(int from_row, int from_col, int to_row, int to_col);
int is_valid_queen_move(int from_row, int from_col, int to_row, int to_col);
int is_valid_king_move(int from_row, int from_col, int to_row, int to_col,
                       int current_player);
int can_castle(int from_row, int from_col, int to_row, int to_col,
               int current_player);
int is_in_check(int king_row, int king_col, int current_player);
int is_checkmate(int king_row, int king_col, int current_player);
int is_valid_move(int from_row, int from_col, int to_row, int to_col,
                  int current_player);
int move_piece(int from_row, int from_col, int to_row, int to_col,
               int current_player);
int parse_move(const char *move_str, int *from_row, int *from_col, int *to_row,
               int *to_col);

#endif