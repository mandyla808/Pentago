#ifndef _LOGIC_H
#define _LOGIC_H

#include "board.h"

enum turn {
    BLACK_NEXT,
    WHITE_NEXT
};

typedef enum turn turn;


enum outcome {
    BLACK_WIN,
    WHITE_WIN,
    DRAW
};

typedef enum outcome outcome;


enum quadrant {
    NW, NE, SW, SE
};

typedef enum quadrant quadrant;

enum direction {
    CW, CCW
};

typedef enum direction direction;


struct game {
    board* b;
    turn next;
};

typedef struct game game;

/* The function new_game should create a new game
 * with the specified size and using the desired 
 * data representation */
game* new_game(unsigned int side, enum type type);

/* The game_free function fully frees a game */
void game_free(game* g);

/* The place_marble function places a marble belonging
 * to the player whose turn it is at the specified location */
int place_marble(game* g, pos p);

/* The twist_quadrant function performs such a twist. */
void twist_quadrant(game* g, quadrant q, direction d);

/* The game_over function should return true or false to
 * indicate whether the game is over according to the rules. */
int game_over(game* g);

/* The game_outcome function should report the actual outcome */
outcome game_outcome(game* g);


/* HELPERS */

/* board_full checks if a board is full */
int board_full(board* b);

/* diagonal_win checks if a player has (n-1) in a row 
 * diagonally */
int diagonal_win(board* b, square color);

/* vertical_win checks if a player has (n-1) in a row 
 * vertically */
int vertical_win(board* b, square color);

/* horizontal_win checks if a player has (n-1) in a row
 * horizontally */
int horizontal_win(board* b,square color);


#endif /* _LOGIC_H */
