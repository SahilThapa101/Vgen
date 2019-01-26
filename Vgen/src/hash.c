#include "hash.h"

int ProbeHash(u8 sideToMove, int depth, int alpha, int beta, Move *bestMove, u64 key) {
    
    if(sideToMove) {
        key ^= KEY_BLACK_TO_MOVE;
    }

    HASHE *phashe = &hashTable[key % HASH_TABLE_SIZE];
    
    if (phashe->key == key) {
        
        if (phashe->depth >= depth) {
            
            if (phashe->flags == hashfEXACT) {
               
			   return phashe->value;
            }
            
            if ((phashe->flags == hashfALPHA) && (phashe->value <= alpha)) {
                
                return alpha;
            }
            
            if ((phashe->flags == hashfBETA) && (phashe->value >= beta)) {
                
                return beta;
            }
		}
		
		Move move;
		move.move = phashe->move;
		move.score = 0;
	
		*bestMove = move;
    }
	    
    return VAL_UNKNOWN;
}

void RecordHash(u8 sideToMove, int depth, int value, int hashf, Move bestMove, u64 key) {
    
    if(sideToMove) {
        key ^= KEY_BLACK_TO_MOVE;
    }

    HASHE *phashe = &hashTable[key % HASH_TABLE_SIZE];
    
    phashe->key = key;
    phashe->move = bestMove.move;
	phashe->value = value;
    phashe->flags = hashf;
    phashe->depth = depth;
}
bool checkForRepetition(u64 key, u8 sideToMove) {

	if(sideToMove) {

		key ^= KEY_BLACK_TO_MOVE;
	}

	for(int i = repIndex - fiftyMoves; i <= repIndex; i++) {

		if(key == repHashTable[i]) {

			return true;
		}
	}

	return false;
}

void insertRepetitionHashKey(u64 key, u8 sideToMove) {
	
	if(sideToMove) {

		key ^= KEY_BLACK_TO_MOVE;
	}

	repIndex++;
	repHashTable[repIndex] = key;
}

void removeRepetitionHashKey() {
	
	repIndex--;
}
