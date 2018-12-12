/*
*
* May 4, 2017
*
*/

#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "globals.h"

void print_bb(u64 board);

void print_board(u64 board);

int bitScanForward(u64 board);

bool isSqAttacked(u8 sq, const u8 color);

int divide(u8 depth, u8 color);

u64 getBitboardFromSquare(int sq);

u64 bbFromAlgebricPos(char* posName);

int popCount (u64 x); 

#endif
