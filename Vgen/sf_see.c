static int full_see(const Board *B, int fsq, int tsq)
// SEE from StockFish: adapted and improved to handle promotions
{
	assert(square_ok(fsq) && square_ok(tsq));

	int stm = color_on(B, fsq);	// side to move
	U64 attackers, stm_attackers;
	int swap_list[32], sl_idx = 1;
	occ_t occ = B->st->occ;

	int piece = B->piece_on[fsq], capture;

	const bool is_prank = test_bit(PPromotionRank[stm], tsq);
	static const int see_val[NB_PIECE+1] = {MP, MN, MB, MR, MQ, 200*MP, 0};

	// Handle en-passant
	if (B->st->epsq == tsq && B->piece_on[fsq] == Pawn) {
		clear_occ(&occ, stm ? tsq+8 : tsq-8);
		capture = Pawn;
	} else
		capture = B->piece_on[tsq];

	assert(capture != King);

	swap_list[0] = see_val[capture];
	clear_occ(&occ, fsq);
	attackers = calc_attackers(B, tsq, &occ);

	// Handle promotion
	if (piece == Pawn && is_prank) {
		swap_list[0] += see_val[Queen] - see_val[Pawn];
		capture = Queen;
	} else
		capture = B->piece_on[fsq];

	// If the opponent has no attackers we are finished
	stm = opp_color(stm);
	stm_attackers = attackers & B->all[stm];
	
	if (!stm_attackers)
		return swap_list[0];

	/* The destination square is defended, which makes things rather more difficult to compute. We
	 * proceed by building up a "swap list" containing the material gain or loss at each stop in a
	 * sequence of captures to the destination square, where the sides alternately capture, and
	 * always capture with the least valuable piece. After each capture, we look for new X-ray
	 * attacks from behind the capturing piece. */
	do {
		/* Locate the least valuable attacker for the side to move. The loop below looks like it is
		 * potentially infinite, but it isn't. We know that the side to move still has at least one
		 * attacker left. */
		for (piece = Pawn; !(stm_attackers & B->b[stm][piece]); piece++)
			assert(piece < King);

		// remove the piece (from wherever it came)
		clear_occ(&occ, first_bit(stm_attackers & B->b[stm][piece]));
		// scan for new X-ray attacks through the vacated square
		attackers |= (get_all_RQ(B) & rook_attack(&occ, tsq)) | (get_all_BQ(B) & bishop_attack(&occ, tsq));
		// cut out pieces we've already done
		attackers &= occ.all;

		// add the new entry to the swap list (beware of promoting pawn captures)
		assert(sl_idx < 32);
		swap_list[sl_idx] = -swap_list[sl_idx - 1] + see_val[capture];
		if (piece == Pawn && is_prank) {
			swap_list[sl_idx] += see_val[Queen] - see_val[Pawn];
			capture = Queen;
		} else
			capture = piece;
		sl_idx++;

		stm = opp_color(stm);
		stm_attackers = attackers & B->all[stm];

		// Stop after a king capture
		if (piece == King && stm_attackers) {
			assert(sl_idx < 32);
			swap_list[sl_idx++] = see_val[King];
			break;
		}
	} while (stm_attackers);

	/* Having built the swap list, we negamax through it to find the best achievable score from the
	 * point of view of the side to move */
	while (--sl_idx)
		swap_list[sl_idx-1] = min(-swap_list[sl_idx], swap_list[sl_idx-1]);

	return swap_list[0];
}