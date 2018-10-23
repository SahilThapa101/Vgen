/*
 * movegen.c
 *
 *  Created on: May 2, 2017
 *      Author: Amar Thapa
 */

#include "Globals.h"
#include "movegen.h"
#include "magicmoves.h"
#include "nonslidingmoves.h"
#include "utility.h"

typedef unsigned char u8;
u64 genMoves(u32 *move_list, u8 color) {

	u8 pos = 0;

	pos = genPushes(move_list, pos, color);
	pos = genAttacks(move_list, pos, color);
	pos = genSpecialMoves(move_list, pos, color);

	return pos;
}

u64 genPushes(u32 *move_list, u8 pos, u8 color) {

	pos = genKingPushes(move_list, pos, color);
	pos = genQueenPushes(move_list, pos, color);
	pos = genBishopPushes(move_list, pos, color);
	pos = genKnightPushes(move_list, pos, color);
	pos = genRookPushes(move_list, pos, color);
	pos = genPawnPushes(move_list, pos, color);
	pos = genDoublePushes(move_list, pos, color);

	return pos;
}

u64 genAttacks(u32 *move_list, u8 pos, u8 color) {

	pos = genKingAttacks(move_list, pos, color);
	pos = genQueenAttacks(move_list, pos, color);
	pos = genBishopAttacks(move_list, pos, color);
	pos = genKnightAttacks(move_list, pos, color);
	pos = genRookAttacks(move_list, pos, color);
	pos = genPawnAttacks(move_list, pos, color);

	return pos;
}

u64 genSpecialMoves(u32 *move_list, u8 pos, u8 color) {

	pos = genCastlingMoves(move_list, pos, color);
	pos = genEnpassantMoves(move_list, pos, color);
	pos = genPromotions(move_list, pos, color);

	return pos;
}

/* pushes aka quiet moves */

u64 genKingPushes(u32 *move_list, u8 pos, u8 color) {

	u64 king_bb = pieceBB[color][KING];

	while (king_bb) {
		const u8 from = bitScanForward(king_bb);
		king_bb &= king_bb - 1;

		u64 pushes = get_king_attacks(from) & empty;

		while (pushes) {
			const u8 to = bitScanForward(pushes);
			pushes &= pushes - 1;

			move_list[pos++] = createMove(0, 0, 0, color, DUMMY, KING, from,
					to);
		}
	}

	return pos;
}

u64 genQueenPushes(u32 *move_list, u8 pos, u8 color) {

	u64 queen_bb = pieceBB[color][QUEEN];

	while (queen_bb) {
		const u8 from = bitScanForward(queen_bb);
		queen_bb &= queen_bb - 1;

		u64 pushes = Qmagic(from, occupied) & empty;

		while (pushes) {
			const u8 to = bitScanForward(pushes);
			pushes &= pushes - 1;

			move_list[pos++] = createMove(0, 0, 0, color, DUMMY, QUEEN, from,
					to);
		}
	}

	return pos;
}

u64 genBishopPushes(u32 *move_list, u8 pos, u8 color) {
	u64 bishops_bb = pieceBB[color][BISHOPS];

	while (bishops_bb) {
		const u8 from = bitScanForward(bishops_bb);
		bishops_bb &= bishops_bb - 1;

		u64 pushes = Bmagic(from, occupied) & empty;

		while (pushes) {
			const u8 to = bitScanForward(pushes);
			pushes &= pushes - 1;

			move_list[pos++] = createMove(0, 0, 0, color, DUMMY, BISHOPS, from,
					to);
		}
	}

	return pos;
}

u64 genKnightPushes(u32 *move_list, u8 pos, u8 color) {

	u64 knights_bb = pieceBB[color][KNIGHTS];

	while (knights_bb) {
		const u8 from = bitScanForward(knights_bb);
		knights_bb &= knights_bb - 1;

		u64 pushes = get_knight_attacks(from) & empty;

		while (pushes) {
			const u8 to = bitScanForward(pushes);
			pushes &= pushes - 1;

			move_list[pos++] = createMove(0, 0, 0, color, DUMMY, KNIGHTS, from,
					to);
		}
	}

	return pos;
}

u64 genRookPushes(u32 *move_list, u8 pos, u8 color) {
	u64 rooks_bb = pieceBB[color][ROOKS];

	while (rooks_bb) {
		const u8 from = bitScanForward(rooks_bb);
		rooks_bb &= rooks_bb - 1;

		u64 pushes = Rmagic(from, occupied) & empty;

		while (pushes) {
			const u8 to = bitScanForward(pushes);
			pushes &= pushes - 1;

			move_list[pos++] = createMove(0, 0, 0, color, DUMMY, ROOKS, from,
					to);
		}
	}

	return pos;
}

u64 genPawnPushes(u32 *move_list, u8 pos, u8 color) {

	u8 from;
	u8 to;
	u64 fromBB;
	u64 toBB;
	u64 pawnsBB = pieceBB[color][PAWNS];
	u64 targetSquares;

	if(color == WHITE) {

		pawnsBB &= NOT_RANK_7;
		targetSquares  =  (pawnsBB << 8) & empty;
	} else {

		pawnsBB &= NOT_RANK_2;
		targetSquares  =  (pawnsBB >> 8) & empty;
	}

	while(targetSquares) {

		to = bitScanForward(targetSquares);

		toBB = getBitboardFromSquare(to);

		if(color == WHITE) {
			fromBB = toBB >> 8;
		} else {
			fromBB = toBB << 8;
		}

		from = bitScanForward(fromBB);

		move_list[pos++] = createMove(0, 0, 0, color, DUMMY, PAWNS, from, to);

		targetSquares &= targetSquares - 1;
	}

	//	u64 pawns_single_push_target_squares = ((pawnsBB << 8) >> (16 * color)) & empty;
	//
	//	u64 pawns_can_push = (pawns_single_push_target_squares >> 8) << (16 * (color));
	//
	//	while (pawns_can_push) {
	//		const u8 from = bit_scan_forward(pawns_can_push);
	//		pawns_can_push &= pawns_can_push - 1;
	//		const u8 to = bit_scan_forward(pawns_single_push_target_squares);
	//		pawns_single_push_target_squares &= pawns_single_push_target_squares
	//				- 1;
	//
	//		move_list[pos++] = create_move(0, 0, 0, color, DUMMY, PAWNS, from, to);
	//	}

	return pos;
}

u64 genDoublePushes(u32 *move_list, u8 pos, u8 color) {

	u64 pawns_bb = pieceBB[color][PAWNS];

	u64 pawns_single_push = ((pawns_bb << 8) >> (16 * color)) & empty;

	u64 pawns_double_push_target_squares = ((pawns_single_push << 8)
			>> (16 * color)) & empty;

	if (color)
		pawns_double_push_target_squares &= RANK_5;
	else
		pawns_double_push_target_squares &= RANK_4;

	u64 pawns_can_push = (pawns_double_push_target_squares >> 16)
									<< (32 * color);

	while (pawns_can_push) {
		const u8 from = bitScanForward(pawns_can_push);
		pawns_can_push &= pawns_can_push - 1;

		const u8 to = bitScanForward(pawns_double_push_target_squares);
		pawns_double_push_target_squares &= pawns_double_push_target_squares
				- 1;

		move_list[pos++] = createMove(0, 0, 2, color, DUMMY, PAWNS, from, to);
	}

	return pos;
}

/* attacks */

u64 genKingAttacks(u32 *move_list, u8 pos, u8 color) {
	u64 king_bb = pieceBB[color][KING];

	while (king_bb) {
		const u8 from = bitScanForward(king_bb);
		king_bb &= king_bb - 1;

		u64 attacks = get_king_attacks(from) & pieceBB[color ^ 1][PIECES];

		while (attacks) {

			const u8 to = bitScanForward(attacks);
			attacks &= attacks - 1;

			if (index_bb[to] & pieceBB[color ^ 1][QUEEN]) {
				move_list[pos++] = createMove(0, 0, 1, color, QUEEN, KING,
						from, to);
			}

			if (pieceBB[color ^ 1][BISHOPS]) {
				u64 bishops_bb = pieceBB[color ^ 1][BISHOPS];
				while (bishops_bb) {
					const u8 sq = bitScanForward(bishops_bb);
					bishops_bb &= bishops_bb - 1;

					if (sq == to)
						move_list[pos++] = createMove(0, 0, 1, color, BISHOPS,
								KING, from, to);
				}
			}

			if (pieceBB[color ^ 1][KNIGHTS]) {
				u64 knights_bb = pieceBB[color ^ 1][KNIGHTS];
				while (knights_bb) {
					const u8 sq = bitScanForward(knights_bb);
					knights_bb &= knights_bb - 1;

					if (sq == to)
						move_list[pos++] = createMove(0, 0, 1, color, KNIGHTS,
								KING, from, to);
				}
			}

			if (pieceBB[color ^ 1][ROOKS]) {
				u64 rooks_bb = pieceBB[color ^ 1][ROOKS];
				while (rooks_bb) {
					const u8 sq = bitScanForward(rooks_bb);
					rooks_bb &= rooks_bb - 1;

					if (sq == to)
						move_list[pos++] = createMove(0, 0, 1, color, ROOKS,
								KING, from, to);
				}
			}

			if (pieceBB[color ^ 1][PAWNS]) {
				u64 pawns_bb = pieceBB[color ^ 1][PAWNS];
				while (pawns_bb) {
					const u8 sq = bitScanForward(pawns_bb);
					pawns_bb &= pawns_bb - 1;

					if (sq == to)
						move_list[pos++] = createMove(0, 0, 1, color, PAWNS,
								KING, from, to);
				}
			}

		}
	}

	return pos;
}

u64 genQueenAttacks(u32 *move_list, u8 pos, u8 color) {
	u64 queen_bb = pieceBB[color][QUEEN];
	while (queen_bb) {
		const u8 from = bitScanForward(queen_bb);
		queen_bb &= queen_bb - 1;

		u64 attacks = Qmagic(from, occupied) & pieceBB[color ^ 1][PIECES];

		while (attacks) {
			const u8 to = bitScanForward(attacks);
			attacks &= attacks - 1;

			if (index_bb[to] & pieceBB[color ^ 1][QUEEN]) {
				move_list[pos++] = createMove(0, 0, 1, color, QUEEN, QUEEN,
						from, to);
			}

			if (pieceBB[color ^ 1][BISHOPS]) {
				u64 bishops_bb = pieceBB[color ^ 1][BISHOPS];
				while (bishops_bb) {
					const u8 sq = bitScanForward(bishops_bb);
					bishops_bb &= bishops_bb - 1;

					if (sq == to)
						move_list[pos++] = createMove(0, 0, 1, color, BISHOPS,
								QUEEN, from, to);
				}
			}

			if (pieceBB[color ^ 1][KNIGHTS]) {
				u64 knights_bb = pieceBB[color ^ 1][KNIGHTS];
				while (knights_bb) {
					const u8 sq = bitScanForward(knights_bb);
					knights_bb &= knights_bb - 1;

					if (sq == to)
						move_list[pos++] = createMove(0, 0, 1, color, KNIGHTS,
								QUEEN, from, to);
				}
			}

			if (pieceBB[color ^ 1][ROOKS]) {
				u64 rooks_bb = pieceBB[color ^ 1][ROOKS];
				while (rooks_bb) {
					const u8 sq = bitScanForward(rooks_bb);
					rooks_bb &= rooks_bb - 1;

					if (sq == to)
						move_list[pos++] = createMove(0, 0, 1, color, ROOKS,
								QUEEN, from, to);
				}
			}

			if (pieceBB[color ^ 1][PAWNS]) {
				u64 pawns_bb = pieceBB[color ^ 1][PAWNS];
				while (pawns_bb) {
					const u8 sq = bitScanForward(pawns_bb);
					pawns_bb &= pawns_bb - 1;

					if (sq == to)
						move_list[pos++] = createMove(0, 0, 1, color, PAWNS,
								QUEEN, from, to);
				}
			}

		}
	}
	return pos;
}

u64 genBishopAttacks(u32 *move_list, u8 pos, u8 color) {
	u64 bishop_bb = pieceBB[color][BISHOPS];

	while (bishop_bb) {
		const u8 from = bitScanForward(bishop_bb);
		bishop_bb &= bishop_bb - 1;

		u64 attacks = Bmagic(from, occupied) & (pieceBB[color ^ 1][PIECES]);

		while (attacks) {

			const u8 to = bitScanForward(attacks);
			attacks &= attacks - 1;

			if (index_bb[to] & pieceBB[color ^ 1][QUEEN]) {
				move_list[pos++] = createMove(0, 0, 1, color, QUEEN, BISHOPS,
						from, to);
			}

			if (pieceBB[color ^ 1][BISHOPS]) {
				u64 bishops_bb = pieceBB[color ^ 1][BISHOPS];
				while (bishops_bb) {
					const u8 sq = bitScanForward(bishops_bb);
					bishops_bb &= bishops_bb - 1;

					if (sq == to)
						move_list[pos++] = createMove(0, 0, 1, color, BISHOPS,
								BISHOPS, from, to);
				}
			}

			if (pieceBB[color ^ 1][KNIGHTS]) {
				u64 knights_bb = pieceBB[color ^ 1][KNIGHTS];
				while (knights_bb) {
					const u8 sq = bitScanForward(knights_bb);
					knights_bb &= knights_bb - 1;

					if (sq == to)
						move_list[pos++] = createMove(0, 0, 1, color, KNIGHTS,
								BISHOPS, from, to);
				}
			}

			if (pieceBB[color ^ 1][ROOKS]) {
				u64 rooks_bb = pieceBB[color ^ 1][ROOKS];
				while (rooks_bb) {
					const u8 sq = bitScanForward(rooks_bb);
					rooks_bb &= rooks_bb - 1;

					if (sq == to)
						move_list[pos++] = createMove(0, 0, 1, color, ROOKS,
								BISHOPS, from, to);
				}
			}

			if (pieceBB[color ^ 1][PAWNS]) {
				u64 pawns_bb = pieceBB[color ^ 1][PAWNS];
				while (pawns_bb) {
					const u8 sq = bitScanForward(pawns_bb);
					pawns_bb &= pawns_bb - 1;

					if (sq == to)
						move_list[pos++] = createMove(0, 0, 1, color, PAWNS,
								BISHOPS, from, to);
				}
			}

		}
	}

	return pos;
}

u64 genKnightAttacks(u32 *move_list, u8 pos, u8 color) {

	u64 knights_bb = pieceBB[color][KNIGHTS];

	while (knights_bb) {
		const u8 from = bitScanForward(knights_bb);
		knights_bb &= knights_bb - 1;

		u64 attacks = get_knight_attacks(from) & (pieceBB[color ^ 1][PIECES]);

		while (attacks) {

			const u8 to = bitScanForward(attacks);
			attacks &= attacks - 1;

			if (index_bb[to] & pieceBB[color ^ 1][QUEEN]) {
				move_list[pos++] = createMove(0, 0, 1, color, QUEEN, KNIGHTS,
						from, to);
			}

			if (pieceBB[color ^ 1][BISHOPS]) {
				u64 bishops_bb = pieceBB[color ^ 1][BISHOPS];
				while (bishops_bb) {
					const u8 sq = bitScanForward(bishops_bb);
					bishops_bb &= bishops_bb - 1;

					if (sq == to)
						move_list[pos++] = createMove(0, 0, 1, color, BISHOPS,
								KNIGHTS, from, to);
				}
			}

			if (pieceBB[color ^ 1][KNIGHTS]) {
				u64 knights_bb = pieceBB[color ^ 1][KNIGHTS];
				while (knights_bb) {
					const u8 sq = bitScanForward(knights_bb);
					knights_bb &= knights_bb - 1;

					if (sq == to)
						move_list[pos++] = createMove(0, 0, 1, color, KNIGHTS,
								KNIGHTS, from, to);
				}
			}

			if (pieceBB[color ^ 1][ROOKS]) {
				u64 rooks_bb = pieceBB[color ^ 1][ROOKS];
				while (rooks_bb) {
					const u8 sq = bitScanForward(rooks_bb);
					rooks_bb &= rooks_bb - 1;

					if (sq == to)
						move_list[pos++] = createMove(0, 0, 1, color, ROOKS,
								KNIGHTS, from, to);
				}
			}

			if (pieceBB[color ^ 1][PAWNS]) {
				u64 pawns_bb = pieceBB[color ^ 1][PAWNS];
				while (pawns_bb) {
					const u8 sq = bitScanForward(pawns_bb);
					pawns_bb &= pawns_bb - 1;

					if (sq == to) {

						move_list[pos++] = createMove(0, 0, 1, color, PAWNS,
								KNIGHTS, from, to);
					}
				}
			}

		}
	}

	return pos;
}

u64 genRookAttacks(u32 *move_list, u8 pos, u8 color) {
	u64 rooks_bb = pieceBB[color][ROOKS];
	while (rooks_bb) {
		const u8 from = bitScanForward(rooks_bb);
		rooks_bb &= rooks_bb - 1;
		u64 attacks = Rmagic(from, occupied) & (pieceBB[color ^ 1][PIECES]);

		while (attacks) {

			const u8 to = bitScanForward(attacks);
			attacks &= attacks - 1;

			if (index_bb[to] & pieceBB[color ^ 1][QUEEN]) {
				move_list[pos++] = createMove(0, 0, 1, color, QUEEN, ROOKS,
						from, to);
			}

			if (pieceBB[color ^ 1][BISHOPS]) {
				u64 bishops_bb = pieceBB[color ^ 1][BISHOPS];
				while (bishops_bb) {
					const u8 sq = bitScanForward(bishops_bb);
					bishops_bb &= bishops_bb - 1;

					if (sq == to) {
						move_list[pos++] = createMove(0, 0, 1, color, BISHOPS,
								ROOKS, from, to);
					}
				}
			}

			if (pieceBB[color ^ 1][KNIGHTS]) {
				u64 knights_bb = pieceBB[color ^ 1][KNIGHTS];
				while (knights_bb) {
					const u8 sq = bitScanForward(knights_bb);
					knights_bb &= knights_bb - 1;

					if (sq == to)
						move_list[pos++] = createMove(0, 0, 1, color, KNIGHTS,
								ROOKS, from, to);
				}
			}

			if (pieceBB[color ^ 1][ROOKS]) {
				u64 rooks_bb = pieceBB[color ^ 1][ROOKS];
				while (rooks_bb) {
					const u8 sq = bitScanForward(rooks_bb);
					rooks_bb &= rooks_bb - 1;

					if (sq == to)
						move_list[pos++] = createMove(0, 0, 1, color, ROOKS,
								ROOKS, from, to);
				}
			}

			if (pieceBB[color ^ 1][PAWNS]) {
				u64 pawns_bb = pieceBB[color ^ 1][PAWNS];

				while (pawns_bb) {
					const u8 sq = bitScanForward(pawns_bb);
					pawns_bb &= pawns_bb - 1;

					if (sq == to) {

						move_list[pos++] = createMove(0, 0, 1, color, PAWNS,
								ROOKS, from, to);
					}
				}
			}

		}
	}

	return pos;
}

u64 genPawnAttacks(u32 *move_list, u8 pos, u8 color) {
	u64 pawnsBB = pieceBB[color][PAWNS];

	if(color == WHITE) {

		pawnsBB &= NOT_RANK_7;
	} else {

		pawnsBB &= NOT_RANK_2;
	}

	while (pawnsBB) {
		const u8 from = bitScanForward(pawnsBB);
		pawnsBB &= pawnsBB - 1;

		u64 attacks;

		if (color) {
			//black
			attacks = ((index_bb[from] >> 7) & NOT_A_FILE)
											| ((index_bb[from] >> 9) & NOT_H_FILE);
		} else {
			attacks = ((index_bb[from] << 7) & NOT_H_FILE)
											| ((index_bb[from] << 9) & NOT_A_FILE);
		}

		while (attacks) {
			const u8 to = bitScanForward(attacks);

			attacks &= attacks - 1;

			if (pieceBB[color ^ 1][PAWNS]) {
				u64 pawns_bb = pieceBB[color ^ 1][PAWNS];
				while (pawns_bb) {
					const u8 sq = bitScanForward(pawns_bb);
					pawns_bb &= pawns_bb - 1;

					if (sq == to) {
						move_list[pos++] = createMove(0, 0, 1, color, PAWNS,
								PAWNS, from, to);
					}
				}
			}

			if (index_bb[to] & pieceBB[color ^ 1][QUEEN]) {
				move_list[pos++] = createMove(0, 0, 1, color, QUEEN, PAWNS,
						from, to);
			}

			if (pieceBB[color ^ 1][BISHOPS]) {
				u64 bishops_bb = pieceBB[color ^ 1][BISHOPS];
				while (bishops_bb) {
					const u8 sq = bitScanForward(bishops_bb);
					bishops_bb &= bishops_bb - 1;

					if (sq == to)
						move_list[pos++] = createMove(0, 0, 1, color, BISHOPS,
								PAWNS, from, to);
				}
			}

			if (pieceBB[color ^ 1][KNIGHTS]) {
				u64 knights_bb = pieceBB[color ^ 1][KNIGHTS];
				while (knights_bb) {
					const u8 sq = bitScanForward(knights_bb);
					knights_bb &= knights_bb - 1;

					if (sq == to)
						move_list[pos++] = createMove(0, 0, 1, color, KNIGHTS,
								PAWNS, from, to);
				}
			}

			if (pieceBB[color ^ 1][ROOKS]) {
				u64 rooks_bb = pieceBB[color ^ 1][ROOKS];
				while (rooks_bb) {
					const u8 sq = bitScanForward(rooks_bb);
					rooks_bb &= rooks_bb - 1;

					if (sq == to)
						move_list[pos++] = createMove(0, 0, 1, color, ROOKS,
								PAWNS, from, to);
				}
			}
		}
	}

	return pos;
}

u64 genCastlingMoves(u32 *move_list, u8 pos, u8 color) {

	if (color == WHITE) {

		if (moveStack[ply].castleFlags & CastleFlagWhiteQueen) {
			u64 wq_sqs = empty & WQ_SIDE_SQS;
			if (wq_sqs == WQ_SIDE_SQS
					&& !(isSqAttacked(2, WHITE) || isSqAttacked(3, WHITE)
							|| isSqAttacked(4, WHITE))) {

				u32 move = createMove(0, 0, 4, 0, ROOKS, KING, 4, 2);
				move_list[pos++] = move;

			}
		}

		if (moveStack[ply].castleFlags & CastleFlagWhiteKing) {
			u64 wk_sqs = empty & WK_SIDE_SQS;

			if (wk_sqs == WK_SIDE_SQS
					&& !(isSqAttacked(4, WHITE) || isSqAttacked(5, WHITE)
							|| isSqAttacked(6, WHITE))) {

				u32 move = createMove(0, 1, 4, 0, ROOKS, KING, 4, 6);
				move_list[pos++] = move;
			}
		}

	} else {

		if (moveStack[ply].castleFlags & CastleFlagBlackQueen) {
			u64 bq_sqs = empty & BQ_SIDE_SQS;

			if (bq_sqs == BQ_SIDE_SQS
					&& !(isSqAttacked(58, BLACK) || isSqAttacked(59, BLACK)
							|| isSqAttacked(60, BLACK))) {

				u32 move = createMove(0, 2, 4, 1, ROOKS, KING, 60, 58);
				move_list[pos++] = move;
			}

		}

		if (moveStack[ply].castleFlags & CastleFlagBlackKing) {

			u64 bk_sqs = empty & BK_SIDE_SQS;

			if (bk_sqs == BK_SIDE_SQS
					&& !(isSqAttacked(60, BLACK) || isSqAttacked(61, BLACK)
							|| isSqAttacked(62, BLACK))) {
				u32 move = createMove(0, 3, 4, 1, ROOKS, KING, 60, 62);
				move_list[pos++] = move;
			}
		}
	}

	return pos;
}

u64 genEnpassantMoves(u32 *move_list, u8 pos, u8 color) {

	if (moveStack[ply].epFlag) {

		u8 from;
		u8 to;
		u32 move;
		u64 target_sqs;
		u64 target_pawns;
		u64 pawns = pieceBB[color][PAWNS];
		u64 epSquare = moveStack[ply].epSquare;

		if (color == WHITE) {
			target_sqs = ((epSquare >> 7) & NOT_A_FILE)
											| ((epSquare >> 9) & NOT_H_FILE);
		} else {
			target_sqs = ((epSquare << 7) & NOT_H_FILE)
											| ((epSquare << 9) & NOT_A_FILE);
		}

		target_pawns = target_sqs & pawns;

		while (target_pawns) {

			from = bitScanForward(target_pawns);

			to = bitScanForward(epSquare);

			target_pawns &= target_pawns - 1;

			move = createMove(0, 0, 3, color, PAWNS, PAWNS, from, to);
			move_list[pos++] = move;

		}
	}

	return pos;
}

u64 genPromotions(u32 *move_list, u8 pos, u8 color) {

	u8 sq;
	u8 from;
	u8 to;
	u64 toAttack;
	u64 toPush;
	u64 fromBB;
	u64 pawns = pieceBB[color][PAWNS];
	u64 pawnsToPromote;
	u64 queenBB;
	u64 rooksBB;
	u64 bishopsBB;
	u64 knightsBB;

	if (color == WHITE) {

		pawnsToPromote = pawns & RANK_7;
	} else {

		pawnsToPromote = pawns & RANK_2;
	}

	while (pawnsToPromote) {

		from = bitScanForward(pawnsToPromote);
		fromBB = getBitboardFromSquare(from);

		if (color == WHITE) {

			toAttack = (((fromBB << 7) & NOT_H_FILE)	| ((fromBB << 9) & NOT_A_FILE));
			toPush = (fromBB << 8) & empty;
		} else {

			toAttack = (((fromBB >> 7) & NOT_A_FILE) | ((fromBB >> 9) & NOT_H_FILE));
			toPush = (fromBB >> 8) & empty;
		}

		while (toAttack) {

			to = bitScanForward(toAttack);
			toAttack &= toAttack - 1;

			if (pieceBB[color ^ 1][QUEEN]) {

				queenBB = pieceBB[color ^ 1][QUEEN];

				while (queenBB) {

					sq = bitScanForward(queenBB);
					queenBB &= queenBB - 1;

					if(sq == to)  {
						move_list[pos++] = createMove(PROMOTE_TO_QUEEN, 0,
								MOVE_TYPE_PROMOTION, color, QUEEN, PAWNS, from, to);
						move_list[pos++] = createMove(PROMOTE_TO_ROOK, 0,
								MOVE_TYPE_PROMOTION, color, QUEEN, PAWNS, from, to);
						move_list[pos++] = createMove(PROMOTE_TO_BISHOP, 0,
								MOVE_TYPE_PROMOTION, color, QUEEN, PAWNS, from, to);
						move_list[pos++] = createMove(PROMOTE_TO_KNIGHT, 0,
								MOVE_TYPE_PROMOTION, color, QUEEN, PAWNS, from, to);
					}
				}
			}

			if (pieceBB[color ^ 1][ROOKS]) {

				rooksBB = pieceBB[color ^ 1][ROOKS];

				while (rooksBB) {

					sq = bitScanForward(rooksBB);
					rooksBB &= rooksBB - 1;

					if (sq == to) {

						move_list[pos++] = createMove(PROMOTE_TO_QUEEN, 0,
								MOVE_TYPE_PROMOTION, color, ROOKS, PAWNS, from, to);
						move_list[pos++] = createMove(PROMOTE_TO_ROOK, 0,
								MOVE_TYPE_PROMOTION, color, ROOKS, PAWNS, from, to);
						move_list[pos++] = createMove(PROMOTE_TO_BISHOP, 0,
								MOVE_TYPE_PROMOTION, color, ROOKS, PAWNS, from, to);
						move_list[pos++] = createMove(PROMOTE_TO_KNIGHT, 0,
								MOVE_TYPE_PROMOTION, color, ROOKS, PAWNS, from, to);
					}
				}
			}

			if (pieceBB[color ^ 1][BISHOPS]) {

				bishopsBB = pieceBB[color ^ 1][BISHOPS];

				while (bishopsBB) {

					sq = bitScanForward(bishopsBB);
					bishopsBB &= bishopsBB - 1;

					if (sq == to) {

						move_list[pos++] = createMove(PROMOTE_TO_QUEEN, 0,
								MOVE_TYPE_PROMOTION, color, BISHOPS, PAWNS, from,
								to);
						move_list[pos++] = createMove(PROMOTE_TO_ROOK, 0,
								MOVE_TYPE_PROMOTION, color, BISHOPS, PAWNS, from,
								to);
						move_list[pos++] = createMove(PROMOTE_TO_BISHOP, 0,
								MOVE_TYPE_PROMOTION, color, BISHOPS, PAWNS, from,
								to);
						move_list[pos++] = createMove(PROMOTE_TO_KNIGHT, 0,
								MOVE_TYPE_PROMOTION, color, BISHOPS, PAWNS, from,
								to);
					}
				}
			}

			if (pieceBB[color ^ 1][KNIGHTS]) {

				knightsBB = pieceBB[color ^ 1][KNIGHTS];

				while (knightsBB) {

					sq = bitScanForward(knightsBB);
					knightsBB &= knightsBB - 1;

					if (sq == to) {

						move_list[pos++] = createMove(PROMOTE_TO_QUEEN, 0,
								MOVE_TYPE_PROMOTION, color, KNIGHTS, PAWNS, from,
								to);
						move_list[pos++] = createMove(PROMOTE_TO_ROOK, 0,
								MOVE_TYPE_PROMOTION, color, KNIGHTS, PAWNS, from,
								to);
						move_list[pos++] = createMove(PROMOTE_TO_BISHOP, 0,
								MOVE_TYPE_PROMOTION, color, KNIGHTS, PAWNS, from,
								to);
						move_list[pos++] = createMove(PROMOTE_TO_KNIGHT, 0,
								MOVE_TYPE_PROMOTION, color, KNIGHTS, PAWNS, from,
								to);
					}
				}
			}

			pawnsToPromote &= pawnsToPromote - 1;
		}

		while (toPush) {
			to = bitScanForward(toPush);
			toPush &= toPush - 1;

			move_list[pos++] = createMove(PROMOTE_TO_QUEEN, 0, MOVE_TYPE_PROMOTION,
					color, DUMMY, PAWNS, from, to);
			move_list[pos++] = createMove(PROMOTE_TO_ROOK, 0, MOVE_TYPE_PROMOTION,
					color, DUMMY, PAWNS, from, to);
			move_list[pos++] = createMove(PROMOTE_TO_BISHOP, 0, MOVE_TYPE_PROMOTION,
					color, DUMMY, PAWNS, from, to);
			move_list[pos++] = createMove(PROMOTE_TO_KNIGHT, 0, MOVE_TYPE_PROMOTION,
					color, DUMMY, PAWNS, from, to);
		}
	}

	return pos;
}

/*
 *	Move Type
 *
 *	0 -> quiet moves
 *	1 -> captures
 *	2 -> pawn double moves
 *	3 -> en_passant
 *	4 -> castling
 *	5 -> promotions
 *	6 -> king moves
 */

/*
 *	Promotion Type
 *
 *	0 - Queen
 *	1 - Rook
 *	2 - Knight
 *	3 - Bishop
 **/

u32 createMove(u8 promotion_type, u8 castleDir, u8 move_type, u8 color,
		u8 c_piece, u8 piece, u8 from, u8 to) {

	return (0ull | (u32) promotion_type << 24 | (u32) castleDir << 22
			| (u32) move_type << 19 | (u32) color << 18 | (u32) c_piece << 15
			| (u32) piece << 12 | (u32) from << 6 | (u32) to);
}

/* Extract data from a move structure */
#define promType(move)   		(move & 0x3000000) >> 24
#define castleDir(move)		(move & 0xC00000) >> 22
#define move_type(move)         (move & 0x380000) >> 19
#define colorType(move)        (move & 0x40000) >> 18
#define cPieceType(move)      (move & 0x38000) >> 15
#define pieceType(move)	    (move & 0x7000) >> 12  	
#define from_sq(move)          	(move & 0xFC0) >> 6
#define to_sq(move)				move & 0x3F

