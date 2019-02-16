//
//  evaluate.h
//  Vgen
//
//  Created by Amar Thapa on 23/12/18.
//  Copyright © 2018 Amar Thapa. All rights reserved.
//

#ifndef evaluate_h
#define evaluate_h

#include "globals.h"

int evaluate(u8 sideToMove);
int pSqTScore(u8 sideToMove);
void initTableDoublePawns();
int numOfDoublePawns(u8 side);
u64 wBackward(u64 wpawns, u64 bpawns);
u64 bBackward(u64 bpawns, u64 wpawns);
int countBackWardPawns(u8 side);

#endif /* evaluate_h */
