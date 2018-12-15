#include <string.h>

#include "search.h"
#include "evaluate.h"
#include "globals.h"
#include "movegen.h"
#include "make_unmake.h"
#include "utility.h"

int search(int depth, u8 color) {

	// if(color == WHITE) {

	// 	return max(depth);
	// } else {

	// 	return min(depth);
	// }

	LINE mainline;
	int score = alphabeta(color, depth, -10000, 10000, &mainline);

	int numberOfMoves = mainline.cmove;
	u32 move;


	char pieceName[2][7] = { { 'K', 'Q', 'B', 'N', 'R', (char) 0, (char) 0 }, { 'k',
		'q', 'b', 'n', 'r', (char) 0, (char) 0 } };

	for(int i = 0; i < numberOfMoves; i++) {
		move = mainline.argmove[i];
		char* from = algebricPos(from_sq(move));
		char* to = algebricPos(to_sq(move));

		printf("%c%s-%s, ", pieceName[colorType(move)][pieceType(move)],
			algebricPos(from_sq(move)), algebricPos(to_sq(move)));
	}

	return score;
}

int max(int depth) {

	u8 numberOfMoves;
	u32 moveList[MAX_MOVES];

	int best = -1000;

	if(depth <= 0) {
		return evaluate();
	}

	ply = ply + 1;
	moveStack[ply].epFlag = moveStack[ply - 1].epFlag;
	moveStack[ply].epSquare = moveStack[ply - 1].epSquare;
	moveStack[ply].castleFlags  =  moveStack[ply - 1].castleFlags;

	numberOfMoves = genMoves(moveList, WHITE);

	// printf("numberOfMoves - %d, side - %d, depth - %d", numberOfMoves, WHITE, depth);
	// printf("\n");

	for(int i = 0; i < numberOfMoves; i++) {

		make_move(moveList[i]);

		if (!isSqAttacked(bitScanForward(pieceBB[WHITE][KING]), WHITE)) {

			int val = min(depth - 1);

			if(val > best) {
				best = val;
			}
		}
		
		unmake_move(moveList[i]);

	}

	ply = ply - 1;
	return best;
}

int min(int depth) {

	u8 numberOfMoves;
	u32 moveList[MAX_MOVES];

	int best = 1000;

	if(depth <= 0) {
		return evaluate();
	}

	ply = ply + 1;
	moveStack[ply].epFlag = moveStack[ply - 1].epFlag;
	moveStack[ply].epSquare = moveStack[ply - 1].epSquare;
	moveStack[ply].castleFlags  =  moveStack[ply - 1].castleFlags;

	numberOfMoves = genMoves(moveList, BLACK);

	// printf("numberOfMoves - %d, side - %d, depth - %d", numberOfMoves, BLACK, depth);
	// printf("\n");

	for(int i = 0; i < numberOfMoves; i++) {

		make_move(moveList[i]);

		if(!isSqAttacked(bitScanForward(pieceBB[BLACK][KING]), BLACK)) {

			int val = max(depth - 1);

			if(val < best) {
				best = val;
			}
		}

		unmake_move(moveList[i]);

	}

	ply = ply - 1;
	return best;
}

int alphabeta(int color, int depth, int alpha, int beta, LINE *pline) {

    if (depth == 0) {
    	pline->cmove = 0;
        return Quies(color, alpha, beta);
    }

    ply = ply + 1;
	moveStack[ply].epFlag = moveStack[ply - 1].epFlag;
	moveStack[ply].epSquare = moveStack[ply - 1].epSquare;
	moveStack[ply].castleFlags  =  moveStack[ply - 1].castleFlags;

    LINE line;

	u32 numberOfMoves;
	u32 moveList[MAX_MOVES];

	numberOfMoves = genMoves(moveList, color);

	for(int i = 0; i < numberOfMoves; i++) {

		make_move(moveList[i]);

		if (!isSqAttacked(bitScanForward(pieceBB[color][KING]), color)) {
		
			int val = -alphabeta(color ^ 1, depth - 1, -beta, -alpha, &line);
			 
			unmake_move(moveList[i]);

			if (val >= beta) {

				ply = ply - 1;
				return beta;
			} 
            	
      		if (val > alpha) {
           		alpha = val;

           		pline->argmove[0] = moveList[i];
           		memcpy(pline->argmove + 1, line.argmove, line.cmove * sizeof(u32));
            	pline->cmove = line.cmove + 1;
			}

			continue;
		}

		unmake_move(moveList[i]);
	}

	ply = ply - 1;

	return alpha;
}

int Quies(int color, int alpha, int beta) {

    int val = evaluate();

    if (val >= beta) {
        return beta;
    }

    if (val > alpha) {
        alpha = val;
    }

    ply = ply + 1;
	moveStack[ply].epFlag = moveStack[ply - 1].epFlag;
	moveStack[ply].epSquare = moveStack[ply - 1].epSquare;
	moveStack[ply].castleFlags  =  moveStack[ply - 1].castleFlags;

	u32 numberOfCaptures;
	u32 moveList[MAX_MOVES];

	numberOfCaptures = genCaptures(moveList, color);

	for(int i = 0; i < numberOfCaptures; i++) {

		make_move(moveList[i]);

		if (!isSqAttacked(bitScanForward(pieceBB[color][KING]), color)) {
		
			val = -Quies(color ^ 1, -beta, -alpha);
			 
			unmake_move(moveList[i]);

			if (val >= beta) {

				ply = ply - 1;
            	return beta;
			}
       		if (val > alpha){
           		alpha = val;
       		}

       		continue;
       	}

		unmake_move(moveList[i]);
	}

	ply = ply - 1;
	
	return alpha;
}










