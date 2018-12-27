//
//  evaluate.c
//  Vgen
//
//  Created by Amar Thapa on 23/12/18.
//  Copyright © 2018 Amar Thapa. All rights reserved.
//

#include "evaluate.h"
#include "utility.h"
#include "movegen.h"
#include "make_unmake.h"

int evaluate(u8 color) {
    
    int whiteScore = 0;
    int blackScore = 0;
    
    whiteScore = popCount(pieceBB[WHITE][KING]) * 2000 +
        popCount(pieceBB[WHITE][QUEEN]) * 900 +
        popCount(pieceBB[WHITE][ROOKS]) * 500 +
        popCount(pieceBB[WHITE][KNIGHTS]) * 300 +
        popCount(pieceBB[WHITE][BISHOPS]) * 300 +
        popCount(pieceBB[WHITE][PAWNS]) * 100;
    
    blackScore = popCount(pieceBB[BLACK][KING]) * 2000 +
        popCount(pieceBB[BLACK][QUEEN]) * 900 +
        popCount(pieceBB[BLACK][ROOKS]) * 500 +
        popCount(pieceBB[BLACK][KNIGHTS]) * 300 +
        popCount(pieceBB[BLACK][BISHOPS]) * 300 +
        popCount(pieceBB[BLACK][PAWNS]) * 100;
    
    int materialAdvantage  = color ? (blackScore - whiteScore) : (whiteScore - blackScore);
    //int mobilityScore = (color ? (getMobility(BLACK) - getMobility(WHITE)) : (getMobility(WHITE) - getMobility(BLACK)));

    return materialAdvantage;// + mobilityScore;
}

u8 getMobility(u8 color) {
    
    u32 moveList[MAX_MOVES];
    
    return genMoves(moveList, color);
}

