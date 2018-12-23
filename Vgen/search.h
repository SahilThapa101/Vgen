//
//  search.h
//  Vgen
//
//  Created by Amar Thapa on 23/12/18.
//  Copyright © 2018 Amar Thapa. All rights reserved.
//

#ifndef search_h
#define search_h

#include "globals.h"

typedef struct tagLINE {
    
    int cmove;              // Number of moves in the line.
    
    u32 argmove[MAX_MOVES];  // The line.
    
} LINE;

void search(u8 color);

// for alphabeta
int alphabeta(int color, int depth, int alpha, int beta, int mate, LINE *pline);

int Quies(int color, int alpha, int beta);

void MVV_LVA(u32 *moveList, u32 numberOfMoves);

void sortByLVA(u32 *moveList, int numberOfMoves);

u64 getZobristKeyForPosition(void);

int ProbeHash(int color, int depth, int alpha, int beta);

void RecordHash(int color, int depth, int val, int hashf);

#endif /* search_h */
