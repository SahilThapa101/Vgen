#ifndef hash_h
#define hash_h

#include "globals.h"

int ProbeHash(u8 sideToMove, int depth, int alpha, int beta, Move *bestMove, u64 key);
void RecordHash(u8 sideToMove, int depth, int val, int hashf, Move bestMove, u64 key);

bool checkForRepetition(u64 key, u8 sideToMove);
void insertRepetitionHashKey(u64 key, u8 sideToMove);
void removeRepetitionHashKey(void);

#endif
