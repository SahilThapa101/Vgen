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
    
    u32 argmove[5000];  // The line.
    
} LINE;

void search(u8 color);
void sortMoves(Move *moveList, u8 sideToMove, u8 numberOfMoves, u32 bestMove, u32 killerMove1, u32 killerMove2);
int alphabeta(u8 sideToMove, char depth, int alpha, int beta, int mate, LINE *pline, bool nullMove);
int Quiescense(u8 sideToMove, int alpha, int beta);
void MVV_LVA(Move *moveList, u8 numberOfMoves);
void sortByLVA(u32 *moveList, int numberOfMoves);
int seeCapture(u32 move, u8 sideToMove);
u8 get_smallest_attacker(u8 square, u8 sideToMove, u8 *from);
int see(u8 toSq, u8 cPiece, u8 frSq, u8 piece, u8 sideToMove);
int seeOld(u8 cPiece, u8 square, u8 sideToMove);
u64 attacksTo(u64 occupied, u8 square, u8 sideToMove);
u64 getLeastValuablePiece(u64 attadef, u8 sideToMove, u8 *piece);
u64 considerXrays(u64 occupied, u8 square);
void debugSEE(u32 move, u8 sideToMove);
#endif /* search_h */
