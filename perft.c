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

	n_moves = gen_moves(move_list, sideToMove);

	for (i = 0; i < n_moves; i++) {

		//		ply++;

//		u32 move = move_list[i];
//		hist_add.move = move;
//		hist_add.ep_sq = hist[ply - 1].ep_sq;
//		hist_add.ep_flag = hist[ply - 1].ep_flag;
//		hist_add.castle_flags = hist[ply - 1].castle_flags;
//		hist[ply] = hist_add;
//		hist[ply] = defaultHist;

		make_move(move_list[i]);

		if (is_sq_attacked(bit_scan_forward(piece_bb[sideToMove][KING]), sideToMove)) {
			// illegal move
		} else {
			nodes += perft(depth - 1, (sideToMove ^ 1));
		}

		unmake_move(move_list[i]);

		//		ply--;

	}


	return nodes;
}
