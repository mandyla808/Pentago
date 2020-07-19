#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "board.h"
#include "logic.h"
#include "pos.h"

/* Helper Function that asks which type and converts the input to enum type */
enum type which_type() {
  char ch;
  printf("Would you like to use CELLS (-c) or BITS (-b)?\n");
  scanf(" -%c", &ch);
  switch (ch) {
  case 'c':
    return CELLS;
  case 'b':
    return BITS;
  default:
    printf("invalid type entered\n");
    return which_type();
  }
}

/*Function that prints who's turn is next */
void whos_turn(turn t){
  switch (t) {
  case BLACK_NEXT:
    printf("Black's Turn: \n");
    break;
  case WHITE_NEXT:
    printf("White's Turn: \n");
    break;
  default:
    fprintf(stderr,"whos_turn error: invalid turn entered\n");
    exit(1);
  }
}

/* Helper function to convert char input to position index */
unsigned int convert_pos(char c) {
  if (atoi(&c)||(c == '0')) {
    return atoi(&c);
  } else if (('A' <= c)&&(c <= 'Z')) {
    return 10 + (c - 'A');
  } else if (('a' <= c)&&(c <= 'z')) {
    return 36 + (c - 'a');
  } else {
    fprintf(stderr, "convert_pos error: unexpected case\n");
    exit(1);
  }
}

/* Helper Function to show game outcome */
void show_outcome(outcome x) {
  switch (x){
  case BLACK_WIN:
    printf("Game Over: Black Wins\n");
    break;
  case WHITE_WIN:
    printf("Game Over: White Wins\n");
    break;
  default:
    printf("Game Over: Draw\n");
  }
}

/* Helper function to check if it is the winner's turn */
int winner_matches(game *g, outcome o) {
  if (g->next == WHITE_NEXT && o == WHITE_WIN) {
    return 1;
  } else if (g->next == BLACK_NEXT && o == BLACK_WIN) {
    return 1;
  } else {
    return 0;
  }
}

/* Helper function to perform take input and perform twist */
void do_twist(game* g) {
  int quad=5;
  char dir;
  printf("\n");
  printf("enter a quardrant(1=NE; 2=NW; 3=SE; 4=SW) and direction(r=CW; l=CCW) to twist:\n");
  scanf(" %d%c", &quad, &dir);
  direction x;
  quadrant y;
  if ((quad < 1)||(quad > 4)||((dir != 'r')&&(dir != 'l'))) {
    printf("invalid quadrant or direction\n");
    do_twist(g);
  }
  if (dir == 'r') {
    x = CW;
  } else if (dir == 'l') {
    x = CCW;
  } else {
    fprintf(stderr, "do_twist error: missed invalid direction\n");
    exit(1);
  }
  switch (quad) {
  case 1:
    y = NW;
    break;
  case 2:
    y = NE;
    break;
  case 3:
    y = SW;
    break;
  case 4:
    y = SE;
    break;
  default:
    fprintf(stderr, "do_twist error: missed invalid quadrant\n");
    exit(1);
  }
  twist_quadrant(g, y, x);
}

/* Main function that runs game */
int main(int argc, char *argv[])
{
  int quit = 0;
  unsigned int s;
  char ch, row, col;
  printf("To start game, enter -s (board size): \n");
  scanf("-%c%d", &ch,&s);
  if (ch != 's') {
    fprintf(stderr,"Did not use -s when declaring board size.\n");
    exit(1);
  }
  game *g = new_game(s, which_type());
  while (!quit) {
    board_show(g->b);
    printf("\n");
    whos_turn(g->next);
    printf("enter row and column for marble placement:\n");
    scanf(" %c%c", &row, &col);
    int a = place_marble(g, make_pos(convert_pos(row), convert_pos(col)));
    if (!a) {
      printf("invalid marble placement\n");
    } else {
      if (game_over(g)) {
	outcome o = game_outcome(g);
	if (winner_matches(g,o)) {
	  board_show(g->b);
	  show_outcome(o);
	  quit = 1;
	} else {
	  board_show(g->b);
	  do_twist(g);
	  if (game_over(g)) {
	    board_show(g->b);
	    show_outcome(game_outcome(g));
	    quit = 1;
	  }
	}
      } else {
	board_show(g->b);
	do_twist(g);
	if (game_over(g)) {
	  board_show(g->b);
	  show_outcome(game_outcome(g));
	  quit = 1;
	}
      }
    }
  }
  game_free(g);
  return 0;
}
