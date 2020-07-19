/* Mandy La, mla1
 * CS152, Spring 2019
 * Project 1
 */

#include <stdlib.h>
#include <stdio.h>
#include "board.h"

board* board_new(unsigned int side, enum type type){
  if (side==0 || (side%2 != 0)) {
    fprintf(stderr, "error board_new: odd or 0 side length\n");
    exit(1);
  }
  unsigned int n=0, m;
  board* new;
  if (type == BITS) {
    new = (board*)malloc(sizeof(board));
    if (side*side*2%32 == 0) {
      m = side*side*2/32;
    } else {
      m = 1 + side*side*2/32;
    }
    unsigned int* res = (unsigned int*)
      malloc(sizeof(unsigned int)*m);
    while (n < m){
      res[n] = 0;
      n++;
    }
    new->u.bits = res;
  } else {
    new = (board*)malloc(sizeof(board));
    square **res = (square**)malloc(sizeof(square*)*side);
    for (m=0; m<side; m++) {
      square *rows = (square*)malloc(sizeof(square)*side);
      for (n=0; n < side; n++) {
	rows[n] = EMPTY;
      }
      res[m] = rows;
    }
    new->u.cells = res;
  }
  new->side = side;
  new->type = type;
  return new;
}

void board_free(board *b){
  unsigned int n = 0;
  switch (b->type){
  case BITS:
    free(b->u.bits);
    free(b);
    break;
  case CELLS:
    while (n < b->side){
      free((b->u.cells)[n]);
      n++;
    }
    free(b->u.cells);
    free(b);
    break;
  }
}

/* Helper function for board_show: prints one row */
void row_show(board* b, unsigned int r){
  if (r <= 9) {
    printf("%d ",r);
  } else if ((9 < r) && (r < 36)) {
    printf("%c ", 'A'+(r-10));
  } else if ((35 < r) && (r < 62)) {
    printf("%c ", 'a'+(r-36));
  } else {
    printf("? ");
  }
  unsigned int n=0;
  for (n = 0; n < b->side; n++) {
    if (n == (b->side)/2) {
      printf(" ");
    }
    switch (board_get(b, make_pos(r,n))) {
    case EMPTY:
      printf(".");
      break;
    case BLACK:
      printf("*");
      break;
    case WHITE:
      printf("o");
      break;
    default:
      fprintf(stderr, "row_show error: invalid square\n");
      exit(1);
    }
  }
}

void board_show(board* b){
  unsigned int n = 0;
  printf("  ");
  while (n < b->side) {
    if (n == (b->side)/2) {
      printf(" ");
    }
    if (n <= 9) {
      printf("%d",n);
    } else if ((9 < n)&&(n < 36)) {
      printf("%c", 'A'+(n-10));
    } else if ((35 < n)&&(n < 62)) {
      printf("%c", 'a'+(n-36));
    } else {
      printf("?");
    }
    n++;
  }
  printf("\n");
  printf("\n");
  for(n=0; n < b->side; n++) {
    if (n == (b->side)/2) {
      printf("\n");
    }
    row_show(b, n);
    printf("\n");
  }
}


square bit_to_square(unsigned int n) {
  switch (n) {
  case 0:
    return EMPTY;
    break;
  case 1:
    return BLACK;
    break;
  case 2:
    return WHITE;
    break;
  default:
    fprintf(stderr, "row_show error: bit == 11");
    exit(1);
  }
}  

square board_get(board* b, pos p) {
  unsigned int n = (p.r*2*(b->side)+p.c*2);
  unsigned int m = n%32;
  switch (b->type){
  case BITS:
    return bit_to_square((b->u.bits)[n/32] >> m & 3);
    break;
  case CELLS:
    return (b->u.cells)[(p.r)][(p.c)];
    break;
  default:
    fprintf(stderr, "invalid type\n");
    exit(1);
  }
}

void board_set(board* b, pos p, square s) {
  unsigned int n = (p.r*2*(b->side)+p.c*2);
  unsigned int m = n%32;
  unsigned int e = (b->u.bits)[n/32];
  unsigned int x = ~(3 << m);
  switch (b->type){
  case BITS:
    if (s == WHITE) {
      e = (e & x) | (2 << m);
    } else if (s == BLACK) {
      e = (e & x) | (1 << m);
    } else {
      e = e & x;
    }
    (b->u.bits)[n/32] = e;
    break;
  case CELLS:
    ((b->u.cells)[(p.r)])[(p.c)] = s;
  }
}
