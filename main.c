#include <stdio.h>
#define BOARD_SIZE 8

char board[BOARD_SIZE][BOARD_SIZE];

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
  printf("success");
}

int main() {
  initialize_board();
  return 0;
}
