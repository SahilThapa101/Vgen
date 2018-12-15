#ifndef __SEARCH_H__
#define __SEARCH_H__

#include "globals.h"

typedef struct tagLINE {

    int cmove;              // Number of moves in the line.

    u32 argmove[MAX_MOVES];  // The line.

}   LINE;



int search(int depth, u8 color);

// for minimax search
int max(int depth);
int min(int depth);

// for alphabeta
int alphabeta(int color, int depth, int alpha, int beta, LINE *pline);

int Quies(int color, int alpha, int beta);

#endif