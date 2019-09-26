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

bool isKingInCheck(u8 sideToMove);

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
void clearRepetitionHashTable(void);
void clearKillerMovesTable(void);
void initHashKey(void);
void clearHistoryTable();
void clearCounterMoveTable();

u64 soutOne (u64 b);
u64 nortOne (u64 b);
u64 eastOne (u64 b);
u64 westOne (u64 b);
u64 noEaOne (u64 b);
u64 soEaOne (u64 b);
u64 soWeOne (u64 b);
u64 noWeOne (u64 b);
u64 wFrontSpans(u64 wpawns);
u64 bRearSpans (u64 bpawns);
u64 bFrontSpans(u64 bpawns);
u64 wRearSpans (u64 wpawns);
u64 nortFill(u64 gen);
u64 soutFill(u64 gen);
u64 wFrontFill(u64 wpawns);
u64 wRearFill (u64 wpawns);
u64 bFrontFill(u64 bpawns);
u64 bRearFill (u64 bpawns);
u64 fileFill(u64 gen);
u64 wEastAttackFrontSpans (u64 wpawns);
u64 wWestAttackFrontSpans (u64 wpawns);
u64 bEastAttackFrontSpans (u64 bpawns);
u64 bWestAttackFrontSpans (u64 bpawns);
u64 wEastAttackRearSpans (u64 wpawns);
u64 wWestAttackRearSpans (u64 wpawns);
u64 bEastAttackRearSpans (u64 bpawns);
u64 bWestAttackRearSpans (u64 bpawns);
u64 eastAttackFileFill (u64 pawns);
u64 westAttackFileFill (u64 pawns);
u64 wPawnEastAttacks(u64 wpawns);
u64 wPawnWestAttacks(u64 wpawns);
u64 bPawnEastAttacks(u64 bpawns);
u64 bPawnWestAttacks(u64 bpawns);



#endif /* utility_h */
