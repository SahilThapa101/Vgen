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
    
    whiteScore = popCount(pieceBB[WHITE][KING]) * 200 +
    popCount(pieceBB[WHITE][QUEEN]) * 9 +
    popCount(pieceBB[WHITE][ROOKS]) * 5 +
    popCount(pieceBB[WHITE][KNIGHTS]) * 3 +
    popCount(pieceBB[WHITE][BISHOPS]) * 3 +
    popCount(pieceBB[WHITE][PAWNS]) * 1;
    
    blackScore = popCount(pieceBB[BLACK][KING]) * 200 +
    popCount(pieceBB[BLACK][QUEEN]) * 9 +
    popCount(pieceBB[BLACK][ROOKS]) * 5 +
    popCount(pieceBB[BLACK][KNIGHTS]) * 3 +
    popCount(pieceBB[BLACK][BISHOPS]) * 3 +
    popCount(pieceBB[BLACK][PAWNS]) * 1;
    
    int result = whiteScore - blackScore; //+ 0.1 * (getMobility(WHITE) - getMobility(BLACK));
    
    if ( color == BLACK ) {
        result = -result;
    }
    
    return result;
}

u8 getMobility(u8 color) {
    
    u32 moveList[MAX_MOVES];
    u8 count = genMoves(moveList, color);
    u8 kingChecks = 0;
    
    for(u8 i = 0; i < count; i++) {
        
        make_move(moveList[i]);
        if (isSqAttacked(bitScanForward(pieceBB[color][KING]), color)) {
            kingChecks++;
        }
        unmake_move(moveList[i]);
    }
    
    return count - kingChecks;
}

