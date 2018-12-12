#include "evaluate.h"
#include "globals.h"
#include "utility.h"

int evaluate() {

	int whiteScore = 0;
	int blackScore = 0;

	whiteScore = popCount(pieceBB[WHITE][KING]) * 100 + 
		popCount(pieceBB[WHITE][QUEEN]) * 10 + 
		popCount(pieceBB[WHITE][ROOKS]) * 5 + 
		popCount(pieceBB[WHITE][KNIGHTS]) * 4 + 
		popCount(pieceBB[WHITE][BISHOPS]) * 4 + 
		popCount(pieceBB[WHITE][PAWNS]) * 1;

	blackScore = popCount(pieceBB[BLACK][KING]) * 100 + 
		popCount(pieceBB[BLACK][QUEEN]) * 10 + 
		popCount(pieceBB[BLACK][ROOKS]) * 5 + 
		popCount(pieceBB[BLACK][KNIGHTS]) * 4 + 
		popCount(pieceBB[BLACK][BISHOPS]) * 4 + 
		popCount(pieceBB[BLACK][PAWNS]) * 1;

	return (whiteScore - blackScore);
}