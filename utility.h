/*
*
* May 4, 2017
*
*/

#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "globals.h"

void print_bb(u64 board);
int bit_scan_forward(u64 board);

bool is_sq_attacked(u8 sq, const u8 color);

int  divide(u8 depth, u8 color);

#endif