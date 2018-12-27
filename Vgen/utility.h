//
//  utility.h
//  Vgen
//
//  Created by Amar Thapa on 23/12/18.
//  Copyright © 2018 Amar Thapa. All rights reserved.
//

#ifndef utility_h
#define utility_h

#include "globals.h"

void print_bb(u64 board);

void print_board(u64 board);

int bitScanForward(u64 board);

bool isKingInCheck(u8 color);

bool isSqAttacked(u8 sq, const u8 color);

int divide(u8 depth, u8 color);

u64 getBitboardFromSquare(int sq);

u64 bbFromAlgebricPos(char* posName);

int popCount (u64 x);

char* algebricPos(u8 sq);

u64 rand64(void);

void clearAllBitBoards(void);

void checkUp(void);

void clearHashTable(void);

#endif /* utility_h */
