#include "hash.h"

bool insertRepetitionHashKey(u64 key, u8 sideToMove) {
	
	if(sideToMove) { 
	
		key ^= KEY_BLACK_TO_MOVE;
	}

	for(int i = fiftyMoves; i < repIndex; i++) {
		
		if(key == repHashTable[i]) {
			
			return true;
		}
	}
	
	repHashTable[repIndex] = key;
	repIndex++;
	
	return false;
}

void removeRepetitionHashKey() {
	repIndex--;
	repHashTable[repIndex] = 0ULL;
}

bool checkForRepetition(u64 key, u8 sideToMove) {

	return false;
}	
