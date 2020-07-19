#ifndef _BOARD_H
#define _BOARD_H

#include "pos.h"


enum square {
    EMPTY,
    BLACK,
    WHITE
};

typedef enum square square;


union board_rep {
    enum square** cells;
    unsigned int* bits;
};

typedef union board_rep board_rep;

enum type {
    CELLS, BITS
};


struct board {
    unsigned int side;
    enum type type;
    board_rep u;
};

typedef struct board board;

/* The board_new function is called to  create a 
 * fully-empty board of the desired size,
 * which must be of acceptable dimensions */
board* board_new(unsigned int side, enum type type);

/* The board_free function fully frees the board, 
 * including whichever internal 
 * representation it is using */
void board_free(board* b);

/* The board_show function prints the board to the screen, 
 * along with row and column headers */
void board_show(board* b);

/* The board_get function retrieves squares within a board */
square board_get(board* b, pos p);

/* The board_set function modifies squares within a board */
void board_set(board* b, pos p, square s);

#endif /* _BOARD_H */
