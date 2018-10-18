#include <stdbool.h>
#include "perft.h"
#include "movegen.h"
#include "make_unmake.h"
#include "utility.h"

u64 perft(u8 depth, u8 color) {

	u32 move_list[MAX_MOVES];

	u64 nodes = 0;
	u8 n_moves, i;
	struct hist defaultHist = { 0, 0, 0, 0 };

	const u8 COLOR = color;

	if (depth == 0) {
		return 1;
	}

//	if(depth == 1)
//		return gen_moves(move_list, COLOR);

	n_moves = gen_moves(move_list, COLOR);

	for (i = 0; i < n_moves; i++) {
		ply++;

		u32 move = move_list[i];
		hist_add.move = move;
		hist_add.ep_sq = hist[ply - 1].ep_sq;
		hist_add.ep_flag = hist[ply - 1].ep_flag;
		hist_add.castle_flags = hist[ply - 1].castle_flags;
		hist[ply] = hist_add;

//		hist[ply] = defaultHist;

		make_move(move_list[i]);

		if (!is_sq_attacked(bit_scan_forward(piece_bb[COLOR][KING]), COLOR)) {
			nodes += perft(depth - 1, COLOR ^ 1);
		} else {
			check++;
		}

		unmake_move(move_list[i]);
		ply--;

	}


	return nodes;
}
