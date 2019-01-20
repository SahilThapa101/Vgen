#ifndef hash_h
#define hash_h

#include "globals.h"

bool insertRepetitionHashKey(u64 key, u8 sideToMove);
void removeRepetitionHashKey(void);
bool checkForRepetition(u64 key, u8 sideToMove);

#endif