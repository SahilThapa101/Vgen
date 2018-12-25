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
int alphabeta(u8 color, int depth, int alpha, int beta, int mate, LINE *pline);

int Quiescense(u8 color, int alpha, int beta);

void MVV_LVA(u32 *moveList, u32 numberOfMoves);

void sortByLVA(u32 *moveList, int numberOfMoves);

u64 getZobristKeyForPosition(void);

int ProbeHash(u8 color, int depth, int alpha, int beta, u32 *bestMove);

void RecordHash(u8 color, int depth, int val, int hashf, u32 bestMove);

int sortByBestMoveFound(u32 *moveList, int count, u32 bestMove);

int NegaMax(u8 color, int depth, int alpha, int beta, LINE *pline);

int NextGuess(int alpha, int beta, u8 subtreeCount);

u32 BNS(u8 color, int depth, int alpha, int beta, LINE *pline);

#endif /* search_h */
