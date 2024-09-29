CC = gcc
CFLAGS = -Wall -Wextra -std=c99
OBJECTS = chess_board.o main.o

chess_game: $(OBJECTS)
	$(CC) $(CFLAGS) -o chess_game $(OBJECTS)

chess_board.o: chess_board.c chess_board.h
	$(CC) $(CFLAGS) -c chess_board.c

main.o: main.c chess_board.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f chess_game $(OBJECTS)
