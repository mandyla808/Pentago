#include <stdio.h>
#include <stdlib.h>
#include "logic.h"

game* new_game(unsigned int side, enum type type) {
  game* new = (game*)malloc(sizeof(game));
  new->b = board_new(side, type);
  new->next = WHITE_NEXT;
  return new;
}

void game_free(game* g) {
  board_free(g->b);
  free(g);
}

int place_marble(game* g, pos p) {
  if (board_get(g->b, p) != EMPTY) {
    return 0;
  }
  if ((p.r >= g->b->side) || (p.c >= g->b->side)) {
    return 0;
  }
  switch(g->next){
  case BLACK_NEXT:
    board_set(g->b, p, BLACK);
    return 1;
  case WHITE_NEXT:
    board_set(g->b, p, WHITE);
    return 1;
  }
}

/* helper function to copy a board */
board *copy(board* b) {
  board* new = board_new(b->side, b->type);
  unsigned int r = 0;
  unsigned int c;
  while (r < (b->side)) {
    for (c=0; c < (b->side); c++) {
      board_set(new, make_pos(r,c),board_get(b,make_pos(r,c)));
    }
    r++;
  }
  return new;
}

/* helper function for CW twist_quadrant */
void twist_cw(game* g, quadrant q) {
  unsigned int r = 0;
  unsigned int c;
  unsigned int s = (g->b->side)/2;
  board* new = copy(g->b);
  while (r < s) {
    for (c=0; c < s; c++) {
      switch(q) {
      case NW:
	board_set(new, make_pos(c,s-1-r), board_get(g->b, make_pos(r,c)));
	break;
      case NE:
	board_set(new, make_pos(c,(2*s-1-r)),board_get(g->b, make_pos(r,c+s)));
	break;
      case SW:
	board_set(new, make_pos(c+s,(g->b->side)-1-(r+s)),
		  board_get(g->b, make_pos(r+s,c)));
	break;
      case SE:
	board_set(new, make_pos(c+s,(3*s-1-(r+s))),
		  board_get(g->b, make_pos(r+s,c+s)));
	break;
      default:
	fprintf(stderr, "twist_cw: switch failed");
	exit(1);
      }
    }
    r++;
  }
  board* ptr = g->b;
  g->b = new;
  board_free(ptr);
}

/* helper function for CCW twist_quadrant */
void twist_ccw(game* g, quadrant q) {
  unsigned int r = 0;
  unsigned int c;
  unsigned int s = (g->b->side)/2;
  board* new = copy(g->b);
  while (r < s) {
    for (c=0; c < s; c++) {
      switch(q) {
      case NW:
	board_set(new, make_pos(s-1-c,r), board_get(g->b, make_pos(r,c)));
	break;
      case NE:
	board_set(new, make_pos(2*s -1-(c+s),r+s),board_get(g->b, make_pos(r,c+s)));
	break;
      case SW:
	board_set(new, make_pos(2*s-1-c,r),
		  board_get(g->b, make_pos(r+s,c)));
	break;
      case SE:
	board_set(new, make_pos(3*s-1-(c+s),r+s),
		  board_get(g->b, make_pos(r+s,c+s)));
	break;
      default:
	fprintf(stderr, "twist_ccw: switch failed");
	exit(1);
      }
    }
    r++;
  }
  board* ptr = g->b;
  g->b = new;
  board_free(ptr);
}

void twist_quadrant(game* g, quadrant q, direction d) {
  switch(d) {
  case CW:
    twist_cw(g, q);
    break;
  case CCW:
    twist_ccw(g, q);
    break;
  }
  switch(g->next) {
  case BLACK_NEXT:
    g->next = WHITE_NEXT;
    break;
  case WHITE_NEXT:
    g->next = BLACK_NEXT;
    break;
  }
}

/* helper function determines if board is full */
int board_full (board* b){
  unsigned int r = 0;
  unsigned int c;
  while (r < b->side) {
    for (c=0; c < b->side; c++) {
      if (board_get(b, make_pos(r,c)) == EMPTY) {
	return 0;
      }
    }
    r++;
  }
  return 1;
}

int horizontal_win (board* b, square color) {
  unsigned int r = 0;
  unsigned int acc = 0;
  unsigned int c;
  while (r < b->side) {
    acc = 0;
    for (c=0; c < b->side; c++) {
      if (board_get(b, make_pos(r,c)) == color) {
	acc++;
      } else {
	acc = 0;
      }
      if (acc == (b->side)-1) {
	return 1;
      }
    }
    r++;
  }
  return 0;
}

int vertical_win (board* b, square color) {
  unsigned int c = 0;
  unsigned int acc = 0;
  unsigned int r;
  while (c < b->side) {
    acc = 0;
    for (r=0; r < b->side; r++) {
      if (board_get(b, make_pos(r,c)) == color) {
	acc++;
      } else {
	acc = 0;
      }
      if (acc == (b->side)-1) {
	return 1;
      }
    }
    c++;
  }
  return 0;
}

int diagonal_win (board* b, square color) {
  unsigned int r = 0;
  unsigned int acc = 0;
  unsigned int count;
  unsigned int c = 0;
  for (count=0; count<2; count++) {
    acc = 0;
    while (r < (b->side)) {
      if (board_get(b, make_pos(r,c)) == color) {
	acc++;
      } else {
	acc = 0;
      }
      if (acc == (b->side) - 1) {
	return 1;
      }
      r++;
      c++;
    }
  }
  r = 0;
  c = 1;
  acc = 0;
  while (c < b->side) {
    if (board_get(b, make_pos(r,c)) == color) {
      acc++;
    } else {
      acc = 0;
    }
    if (acc == (b->side) - 1) {
      return 1;
    }
    r++;
    c++;
  }
  r = (b->side) - 1;
  c = 0;
  for (count=0; count < 2; count++) {
    acc = 0;
    while (c < b->side) {
      if (board_get(b, make_pos(r,c)) == color) {
	acc++;
      } else {
	acc = 0;
      }
      if (acc == (b->side) - 1) {
	return 1;
      }
      r--;
      c++;
    }
    r = (b->side) - 2;
  }
  r = (b->side) - 1;
  c = 1;
  acc = 0;
  while (c < b->side) {
    if (board_get(b, make_pos(r,c)) == color) {
      acc++;
    } else {
      acc = 0;
    }
    if (acc == (b->side) - 1) {
      return 1;
    }
    r--;
    c++;
  }
  return 0;
}

int game_over(game* g) {
  int a = board_full(g->b);
  int b = horizontal_win(g->b,WHITE);
  int c = vertical_win(g->b,WHITE);
  int d = diagonal_win(g->b,WHITE);
  int e = horizontal_win(g->b,BLACK);
  int f = vertical_win(g->b,BLACK);
  int h = diagonal_win(g->b,BLACK);
  if (a || b || c || d || e || f || h) {
    return 1;
  } else {
    return 0;
  }
}

outcome game_outcome(game* g) {
  int b = horizontal_win(g->b,WHITE);
  int c = vertical_win(g->b,WHITE);
  int d = diagonal_win(g->b,WHITE);
  int e = horizontal_win(g->b,BLACK);
  int f = vertical_win(g->b,BLACK);
  int h = diagonal_win(g->b,BLACK);
  int white_win = (b || c || d);
  int black_win = (e || f || h);
  if (white_win && black_win){
    return DRAW;
  } else if (white_win) {
    return WHITE_WIN;
  } else if (black_win) {
    return BLACK_WIN;
  } else {
    return DRAW;
  }
}
