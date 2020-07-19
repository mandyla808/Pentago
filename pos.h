#ifndef _POS_H
#define _POS_H

struct pos {
    unsigned int r, c;
};

typedef struct pos pos;

/* a convenience function to create a pos value. */
pos make_pos(unsigned int r, unsigned int c);

#endif /* _POS_H */
