#include <stdbool.h>
#include "perft.h"
#include "movegen.h"
#include "make_unmake.h"
#include "utility.h"

u64 perft(u8 depth, u8 color) {

	u32 move_list[MAX_MOVES];
	u64 nodes = 0;
	u8 n_moves;
	u8 i;
	u8 sideToMove = color;

	struct hist defaultHist = { 0, 0, 0, 0 };

	if (depth == 0) {
		return 1;
	}

	ply = ply + 1;
	moveStack[ply].epFlag = moveStack[ply - 1].epFlag;
	moveStack[ply].epSquare = moveStack[ply - 1].epSquare;
	moveStack[ply].castleFlags  =  moveStack[ply - 1].castleFlags;

//	printf("%x, ", moveStack[ply].castleFlags);

	n_moves = genMoves(move_list, sideToMove);

	for (i = 0; i < n_moves; i++) {



		make_move(move_list[i]);

		if (isSqAttacked(bitScanForward(pieceBB[sideToMove][KING]),
				sideToMove)) {
			// illegal move
			if(move_type(move_list[i]) == 4) {
				cas--;
			}
			if(move_type(move_list[i]) == 1) {
				cap--;
			}
			if(move_type(move_list[i]) == 3) {
				ep--;
			}
			if(move_type(move_list[i]) == 5) {
				prom--;
			}
		} else {

			nodes += perft(depth - 1, (sideToMove ^ 1));
		}

		unmake_move(move_list[i]);
	}

	ply = ply - 1;

	return nodes;
}
