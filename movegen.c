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
u64 gen_moves(u32 *move_list, u8 color) {

	u8 pos = 0;

	pos = gen_pushes(move_list, pos, color);
	pos = gen_attacks(move_list, pos, color);
	pos = gen_special_moves(move_list, pos, color);

	return pos;
}

u64 gen_pushes(u32 *move_list, u8 pos, u8 color) {

	pos = gen_king_pushes(move_list, pos, color);
	pos = gen_queen_pushes(move_list, pos, color);
	pos = gen_bishop_pushes(move_list, pos, color);
	pos = gen_knight_pushes(move_list, pos, color);
	pos = gen_rook_pushes(move_list, pos, color);
	pos = gen_pawn_pushes(move_list, pos, color);
	pos = gen_double_pushes(move_list, pos, color);

	return pos;
}

u64 gen_attacks(u32 *move_list, u8 pos, u8 color) {

	pos = gen_king_attacks(move_list, pos, color);
	pos = gen_queen_attacks(move_list, pos, color);
	pos = gen_bishop_attacks(move_list, pos, color);
	pos = gen_knight_attacks(move_list, pos, color);
	pos = gen_rook_attacks(move_list, pos, color);
	pos = gen_pawn_attacks(move_list, pos, color);

	return pos;
}

u64 gen_special_moves(u32 *move_list, u8 pos, u8 color) {

	pos = gen_castling_moves(move_list, pos, color);
	pos = gen_enpassant_moves(move_list, pos, color);
	//pos = gen_promotions(move_list, pos, color);

	return pos;
}

/* pushes aka quiet moves */

u64 gen_king_pushes(u32 *move_list, u8 pos, u8 color) {

	u64 king_bb = piece_bb[color][KING];

	while (king_bb) {
		const u8 from = bit_scan_forward(king_bb);
		king_bb &= king_bb - 1;

		u64 pushes = get_king_attacks(from) & empty;

		while (pushes) {
			const u8 to = bit_scan_forward(pushes);
			pushes &= pushes - 1;

			move_list[pos++] = create_move(0, 0, 0, color, DUMMY, KING, from,
					to);
		}
	}

	return pos;
}

u64 gen_queen_pushes(u32 *move_list, u8 pos, u8 color) {

	u64 queen_bb = piece_bb[color][QUEEN];

	while (queen_bb) {
		const u8 from = bit_scan_forward(queen_bb);
		queen_bb &= queen_bb - 1;

		u64 pushes = Qmagic(from, occupied) & empty;

		while (pushes) {
			const u8 to = bit_scan_forward(pushes);
			pushes &= pushes - 1;

			move_list[pos++] = create_move(0, 0, 0, color, DUMMY, QUEEN, from,
					to);
		}
	}

	return pos;
}

u64 gen_bishop_pushes(u32 *move_list, u8 pos, u8 color) {
	u64 bishops_bb = piece_bb[color][BISHOPS];

	while (bishops_bb) {
		const u8 from = bit_scan_forward(bishops_bb);
		bishops_bb &= bishops_bb - 1;

		u64 pushes = Bmagic(from, occupied) & empty;

		while (pushes) {
			const u8 to = bit_scan_forward(pushes);
			pushes &= pushes - 1;

			move_list[pos++] = create_move(0, 0, 0, color, DUMMY, BISHOPS, from,
					to);
		}
	}

	return pos;
}

u64 gen_knight_pushes(u32 *move_list, u8 pos, u8 color) {

	u64 knights_bb = piece_bb[color][KNIGHTS];

	while (knights_bb) {
		const u8 from = bit_scan_forward(knights_bb);
		knights_bb &= knights_bb - 1;

		u64 pushes = get_knight_attacks(from) & empty;

		while (pushes) {
			const u8 to = bit_scan_forward(pushes);
			pushes &= pushes - 1;

			move_list[pos++] = create_move(0, 0, 0, color, DUMMY, KNIGHTS, from,
					to);
		}
	}

	return pos;
}

u64 gen_rook_pushes(u32 *move_list, u8 pos, u8 color) {
	u64 rooks_bb = piece_bb[color][ROOKS];

	while (rooks_bb) {
		const u8 from = bit_scan_forward(rooks_bb);
		rooks_bb &= rooks_bb - 1;

		u64 pushes = Rmagic(from, occupied) & empty;

		while (pushes) {
			const u8 to = bit_scan_forward(pushes);
			pushes &= pushes - 1;

			move_list[pos++] = create_move(0, 0, 0, color, DUMMY, ROOKS, from,
					to);
		}
	}

	return pos;
}

u64 gen_pawn_pushes(u32 *move_list, u8 pos, u8 color) {
	u64 pawns_bb = piece_bb[color][PAWNS];
	u64 pawns_single_push_target_squares = ((pawns_bb << 8) >> (16 * (color)))
			& empty;
	u64 pawns_can_push = (pawns_single_push_target_squares >> 8)
			<< (16 * (color));

	while (pawns_can_push) {
		const u8 from = bit_scan_forward(pawns_can_push);
		pawns_can_push &= pawns_can_push - 1;
		const u8 to = bit_scan_forward(pawns_single_push_target_squares);
		pawns_single_push_target_squares &= pawns_single_push_target_squares
				- 1;

		move_list[pos++] = create_move(0, 0, 0, color, DUMMY, PAWNS, from, to);

	}

	return pos;
}

u64 gen_double_pushes(u32 *move_list, u8 pos, u8 color) {

	u64 pawns_bb = piece_bb[color][PAWNS];

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
		const u8 from = bit_scan_forward(pawns_can_push);
		pawns_can_push &= pawns_can_push - 1;

		const u8 to = bit_scan_forward(pawns_double_push_target_squares);
		pawns_double_push_target_squares &= pawns_double_push_target_squares
				- 1;

		move_list[pos++] = create_move(0, 0, 2, color, DUMMY, PAWNS, from, to);
	}

	return pos;
}

/* attacks */

u64 gen_king_attacks(u32 *move_list, u8 pos, u8 color) {
	u64 king_bb = piece_bb[color][KING];

	while (king_bb) {
		const u8 from = bit_scan_forward(king_bb);
		king_bb &= king_bb - 1;

		u64 attacks = get_king_attacks(from) & piece_bb[color ^ 1][PIECES];

		while (attacks) {

			const u8 to = bit_scan_forward(attacks);
			attacks &= attacks - 1;

			if (index_bb[to] & piece_bb[color ^ 1][QUEEN]) {
				move_list[pos++] = create_move(0, 0, 1, color, QUEEN, KING,
						from, to);
			}

			if (piece_bb[color ^ 1][BISHOPS]) {
				u64 bishops_bb = piece_bb[color ^ 1][BISHOPS];
				while (bishops_bb) {
					const u8 sq = bit_scan_forward(bishops_bb);
					bishops_bb &= bishops_bb - 1;

					if (sq == to)
						move_list[pos++] = create_move(0, 0, 1, color, BISHOPS,
								KING, from, to);
				}
			}

			if (piece_bb[color ^ 1][KNIGHTS]) {
				u64 knights_bb = piece_bb[color ^ 1][KNIGHTS];
				while (knights_bb) {
					const u8 sq = bit_scan_forward(knights_bb);
					knights_bb &= knights_bb - 1;

					if (sq == to)
						move_list[pos++] = create_move(0, 0, 1, color, KNIGHTS,
								KING, from, to);
				}
			}

			if (piece_bb[color ^ 1][ROOKS]) {
				u64 rooks_bb = piece_bb[color ^ 1][ROOKS];
				while (rooks_bb) {
					const u8 sq = bit_scan_forward(rooks_bb);
					rooks_bb &= rooks_bb - 1;

					if (sq == to)
						move_list[pos++] = create_move(0, 0, 1, color, ROOKS,
								KING, from, to);
				}
			}

			if (piece_bb[color ^ 1][PAWNS]) {
				u64 pawns_bb = piece_bb[color ^ 1][PAWNS];
				while (pawns_bb) {
					const u8 sq = bit_scan_forward(pawns_bb);
					pawns_bb &= pawns_bb - 1;

					if (sq == to)
						move_list[pos++] = create_move(0, 0, 1, color, PAWNS,
								KING, from, to);
				}
			}

		}
	}

	return pos;
}

u64 gen_queen_attacks(u32 *move_list, u8 pos, u8 color) {
	u64 queen_bb = piece_bb[color][QUEEN];
	while (queen_bb) {
		const u8 from = bit_scan_forward(queen_bb);
		queen_bb &= queen_bb - 1;

		u64 attacks = Qmagic(from, occupied) & piece_bb[color ^ 1][PIECES];

		while (attacks) {
			const u8 to = bit_scan_forward(attacks);
			attacks &= attacks - 1;

			if (index_bb[to] & piece_bb[color ^ 1][QUEEN]) {
				move_list[pos++] = create_move(0, 0, 1, color, QUEEN, QUEEN,
						from, to);
			}

			if (piece_bb[color ^ 1][BISHOPS]) {
				u64 bishops_bb = piece_bb[color ^ 1][BISHOPS];
				while (bishops_bb) {
					const u8 sq = bit_scan_forward(bishops_bb);
					bishops_bb &= bishops_bb - 1;

					if (sq == to)
						move_list[pos++] = create_move(0, 0, 1, color, BISHOPS,
								QUEEN, from, to);
				}
			}

			if (piece_bb[color ^ 1][KNIGHTS]) {
				u64 knights_bb = piece_bb[color ^ 1][KNIGHTS];
				while (knights_bb) {
					const u8 sq = bit_scan_forward(knights_bb);
					knights_bb &= knights_bb - 1;

					if (sq == to)
						move_list[pos++] = create_move(0, 0, 1, color, KNIGHTS,
								QUEEN, from, to);
				}
			}

			if (piece_bb[color ^ 1][ROOKS]) {
				u64 rooks_bb = piece_bb[color ^ 1][ROOKS];
				while (rooks_bb) {
					const u8 sq = bit_scan_forward(rooks_bb);
					rooks_bb &= rooks_bb - 1;

					if (sq == to)
						move_list[pos++] = create_move(0, 0, 1, color, ROOKS,
								QUEEN, from, to);
				}
			}

			if (piece_bb[color ^ 1][PAWNS]) {
				u64 pawns_bb = piece_bb[color ^ 1][PAWNS];
				while (pawns_bb) {
					const u8 sq = bit_scan_forward(pawns_bb);
					pawns_bb &= pawns_bb - 1;

					if (sq == to)
						move_list[pos++] = create_move(0, 0, 1, color, PAWNS,
								QUEEN, from, to);
				}
			}

		}
	}
	return pos;
}

u64 gen_bishop_attacks(u32 *move_list, u8 pos, u8 color) {
	u64 bishop_bb = piece_bb[color][BISHOPS];

	while (bishop_bb) {
		const u8 from = bit_scan_forward(bishop_bb);
		bishop_bb &= bishop_bb - 1;

		u64 attacks = Bmagic(from, occupied) & (piece_bb[color ^ 1][PIECES]);

		while (attacks) {

			const u8 to = bit_scan_forward(attacks);
			attacks &= attacks - 1;

			if (index_bb[to] & piece_bb[color ^ 1][QUEEN]) {
				move_list[pos++] = create_move(0, 0, 1, color, QUEEN, BISHOPS,
						from, to);
			}

			if (piece_bb[color ^ 1][BISHOPS]) {
				u64 bishops_bb = piece_bb[color ^ 1][BISHOPS];
				while (bishops_bb) {
					const u8 sq = bit_scan_forward(bishops_bb);
					bishops_bb &= bishops_bb - 1;

					if (sq == to)
						move_list[pos++] = create_move(0, 0, 1, color, BISHOPS,
								BISHOPS, from, to);
				}
			}

			if (piece_bb[color ^ 1][KNIGHTS]) {
				u64 knights_bb = piece_bb[color ^ 1][KNIGHTS];
				while (knights_bb) {
					const u8 sq = bit_scan_forward(knights_bb);
					knights_bb &= knights_bb - 1;

					if (sq == to)
						move_list[pos++] = create_move(0, 0, 1, color, KNIGHTS,
								BISHOPS, from, to);
				}
			}

			if (piece_bb[color ^ 1][ROOKS]) {
				u64 rooks_bb = piece_bb[color ^ 1][ROOKS];
				while (rooks_bb) {
					const u8 sq = bit_scan_forward(rooks_bb);
					rooks_bb &= rooks_bb - 1;

					if (sq == to)
						move_list[pos++] = create_move(0, 0, 1, color, ROOKS,
								BISHOPS, from, to);
				}
			}

			if (piece_bb[color ^ 1][PAWNS]) {
				u64 pawns_bb = piece_bb[color ^ 1][PAWNS];
				while (pawns_bb) {
					const u8 sq = bit_scan_forward(pawns_bb);
					pawns_bb &= pawns_bb - 1;

					if (sq == to)
						move_list[pos++] = create_move(0, 0, 1, color, PAWNS,
								BISHOPS, from, to);
				}
			}

		}
	}

	return pos;
}

u64 gen_knight_attacks(u32 *move_list, u8 pos, u8 color) {

	u64 knights_bb = piece_bb[color][KNIGHTS];

	while (knights_bb) {
		const u8 from = bit_scan_forward(knights_bb);
		knights_bb &= knights_bb - 1;

		u64 attacks = get_knight_attacks(from) & (piece_bb[color ^ 1][PIECES]);

		while (attacks) {

			const u8 to = bit_scan_forward(attacks);
			attacks &= attacks - 1;

			if (index_bb[to] & piece_bb[color ^ 1][QUEEN]) {
				move_list[pos++] = create_move(0, 0, 1, color, QUEEN, KNIGHTS,
						from, to);
			}

			if (piece_bb[color ^ 1][BISHOPS]) {
				u64 bishops_bb = piece_bb[color ^ 1][BISHOPS];
				while (bishops_bb) {
					const u8 sq = bit_scan_forward(bishops_bb);
					bishops_bb &= bishops_bb - 1;

					if (sq == to)
						move_list[pos++] = create_move(0, 0, 1, color, BISHOPS,
								KNIGHTS, from, to);
				}
			}

			if (piece_bb[color ^ 1][KNIGHTS]) {
				u64 knights_bb = piece_bb[color ^ 1][KNIGHTS];
				while (knights_bb) {
					const u8 sq = bit_scan_forward(knights_bb);
					knights_bb &= knights_bb - 1;

					if (sq == to)
						move_list[pos++] = create_move(0, 0, 1, color, KNIGHTS,
								KNIGHTS, from, to);
				}
			}

			if (piece_bb[color ^ 1][ROOKS]) {
				u64 rooks_bb = piece_bb[color ^ 1][ROOKS];
				while (rooks_bb) {
					const u8 sq = bit_scan_forward(rooks_bb);
					rooks_bb &= rooks_bb - 1;

					if (sq == to)
						move_list[pos++] = create_move(0, 0, 1, color, ROOKS,
								KNIGHTS, from, to);
				}
			}

			if (piece_bb[color ^ 1][PAWNS]) {
				u64 pawns_bb = piece_bb[color ^ 1][PAWNS];
				while (pawns_bb) {
					const u8 sq = bit_scan_forward(pawns_bb);
					pawns_bb &= pawns_bb - 1;

					if (sq == to) {

						move_list[pos++] = create_move(0, 0, 1, color, PAWNS,
								KNIGHTS, from, to);
					}
				}
			}

		}
	}

	return pos;
}

u64 gen_rook_attacks(u32 *move_list, u8 pos, u8 color) {
	u64 rooks_bb = piece_bb[color][ROOKS];
	while (rooks_bb) {
		const u8 from = bit_scan_forward(rooks_bb);
		rooks_bb &= rooks_bb - 1;
		u64 attacks = Rmagic(from, occupied) & (piece_bb[color ^ 1][PIECES]);

		while (attacks) {

			const u8 to = bit_scan_forward(attacks);
			attacks &= attacks - 1;

			if (index_bb[to] & piece_bb[color ^ 1][QUEEN]) {
				move_list[pos++] = create_move(0, 0, 1, color, QUEEN, ROOKS,
						from, to);
			}

			if (piece_bb[color ^ 1][BISHOPS]) {
				u64 bishops_bb = piece_bb[color ^ 1][BISHOPS];
				while (bishops_bb) {
					const u8 sq = bit_scan_forward(bishops_bb);
					bishops_bb &= bishops_bb - 1;

					if (sq == to) {
						move_list[pos++] = create_move(0, 0, 1, color, BISHOPS,
								ROOKS, from, to);
					}
				}
			}

			if (piece_bb[color ^ 1][KNIGHTS]) {
				u64 knights_bb = piece_bb[color ^ 1][KNIGHTS];
				while (knights_bb) {
					const u8 sq = bit_scan_forward(knights_bb);
					knights_bb &= knights_bb - 1;

					if (sq == to)
						move_list[pos++] = create_move(0, 0, 1, color, KNIGHTS,
								ROOKS, from, to);
				}
			}

			if (piece_bb[color ^ 1][ROOKS]) {
				u64 rooks_bb = piece_bb[color ^ 1][ROOKS];
				while (rooks_bb) {
					const u8 sq = bit_scan_forward(rooks_bb);
					rooks_bb &= rooks_bb - 1;

					if (sq == to)
						move_list[pos++] = create_move(0, 0, 1, color, ROOKS,
								ROOKS, from, to);
				}
			}

			if (piece_bb[color ^ 1][PAWNS]) {
				u64 pawns_bb = piece_bb[color ^ 1][PAWNS];

				while (pawns_bb) {
					const u8 sq = bit_scan_forward(pawns_bb);
					pawns_bb &= pawns_bb - 1;

					if (sq == to) {

						move_list[pos++] = create_move(0, 0, 1, color, PAWNS,
								ROOKS, from, to);
					}
				}
			}

		}
	}

	return pos;
}

u64 gen_pawn_attacks(u32 *move_list, u8 pos, u8 color) {
	u64 pawns_bb = piece_bb[color][PAWNS];

	while (pawns_bb) {
		const u8 from = bit_scan_forward(pawns_bb);
		pawns_bb &= pawns_bb - 1;

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
			const u8 to = bit_scan_forward(attacks);

			attacks &= attacks - 1;

			if (piece_bb[color ^ 1][PAWNS]) {
				u64 pawns_bb = piece_bb[color ^ 1][PAWNS];
				while (pawns_bb) {
					const u8 sq = bit_scan_forward(pawns_bb);
					pawns_bb &= pawns_bb - 1;

					if (sq == to) {
						move_list[pos++] = create_move(0, 0, 1, color, PAWNS,
								PAWNS, from, to);
					}
				}
			}

			if (index_bb[to] & piece_bb[color ^ 1][QUEEN]) {
				move_list[pos++] = create_move(0, 0, 1, color, QUEEN, PAWNS,
						from, to);
			}

			if (piece_bb[color ^ 1][BISHOPS]) {
				u64 bishops_bb = piece_bb[color ^ 1][BISHOPS];
				while (bishops_bb) {
					const u8 sq = bit_scan_forward(bishops_bb);
					bishops_bb &= bishops_bb - 1;

					if (sq == to)
						move_list[pos++] = create_move(0, 0, 1, color, BISHOPS,
								PAWNS, from, to);
				}
			}

			if (piece_bb[color ^ 1][KNIGHTS]) {
				u64 knights_bb = piece_bb[color ^ 1][KNIGHTS];
				while (knights_bb) {
					const u8 sq = bit_scan_forward(knights_bb);
					knights_bb &= knights_bb - 1;

					if (sq == to)
						move_list[pos++] = create_move(0, 0, 1, color, KNIGHTS,
								PAWNS, from, to);
				}
			}

			if (piece_bb[color ^ 1][ROOKS]) {
				u64 rooks_bb = piece_bb[color ^ 1][ROOKS];
				while (rooks_bb) {
					const u8 sq = bit_scan_forward(rooks_bb);
					rooks_bb &= rooks_bb - 1;

					if (sq == to)
						move_list[pos++] = create_move(0, 0, 1, color, ROOKS,
								PAWNS, from, to);
				}
			}
		}
	}

	return pos;
}

u64 gen_castling_moves(u32 *move_list, u8 pos, u8 color) {

		if (color == WHITE) {

			if (moveStack[ply].castleFlags & CastleFlagWhiteQueen) {
				u64 wq_sqs = empty & WQ_SIDE_SQS;
				if (wq_sqs == WQ_SIDE_SQS && !(is_sq_attacked(2, WHITE) || is_sq_attacked(3, WHITE) || is_sq_attacked(4, WHITE))) {

					u32 move = create_move(0, 0, 4, 0, ROOKS, KING, 4, 2);
					move_list[pos++] = move;

				}
			}

			if (moveStack[ply].castleFlags & CastleFlagWhiteKing) {
				u64 wk_sqs = empty & WK_SIDE_SQS;

				if (wk_sqs == WK_SIDE_SQS && !(is_sq_attacked(4, WHITE) || is_sq_attacked(5, WHITE) || is_sq_attacked(6, WHITE))) {

					u32 move = create_move(0, 1, 4, 0, ROOKS, KING, 4, 6);
					move_list[pos++] = move;
				}
			}

		} else {

			if (moveStack[ply].castleFlags & CastleFlagBlackQueen) {
				u64 bq_sqs = empty & BQ_SIDE_SQS;

				if (bq_sqs == BQ_SIDE_SQS && !(is_sq_attacked(58, BLACK)
						|| is_sq_attacked(59, BLACK) || is_sq_attacked(60, BLACK))) {

					u32 move = create_move(0, 2, 4, 1, ROOKS, KING, 60, 58);
					move_list[pos++] = move;
				}

			}

			if (moveStack[ply].castleFlags & CastleFlagBlackKing) {

				u64 bk_sqs = empty & BK_SIDE_SQS;

				if (bk_sqs == BK_SIDE_SQS && !(is_sq_attacked(60, BLACK) || is_sq_attacked(61, BLACK) || is_sq_attacked(62, BLACK))) {
					u32 move = create_move(0, 3, 4, 1, ROOKS, KING, 60, 62);
					move_list[pos++] = move;
				}
			}
		}

	return pos;
}

u64 gen_enpassant_moves(u32 *move_list, u8 pos, u8 color) {

	if (moveStack[ply].epFlag) {

		u8 from;
		u8 to;
		u32 move;
		u64 target_sqs;
		u64 target_pawns;
		u64 pawns = piece_bb[color][PAWNS];
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

			from = bit_scan_forward(target_pawns);

			to = bit_scan_forward(epSquare);

			target_pawns &= target_pawns - 1;

			move = create_move(0, 0, 3, color, PAWNS, PAWNS, from, to);
			move_list[pos++] = move;

		}
	}

	return pos;
}

u64 gen_promotions(u32 *move_list, u8 pos, u8 color) {
	u64 prom_pawns = piece_bb[color ^ 1][PAWNS];

	if (color ^ 1) {
		prom_pawns &= RANK_2;
	} else {
		prom_pawns &= RANK_7;
	}

	while (prom_pawns) {

		const u8 from = bit_scan_forward(prom_pawns);
		prom_pawns &= prom_pawns - 1;

		u8 to_attack = (from << 7) >> (14 * (color ^ 1))
				| (from << 9) >> (16 * (color ^ 1));

		while (to_attack) {
			const u8 to = bit_scan_forward(to_attack);
			to_attack &= to_attack - 1;

			if (to & piece_bb[color][QUEEN]) {
				u64 queen_prom = create_move(0, 0, 5, color ^ 1, QUEEN, PAWNS,
						from, to);
				u64 rook_prom = create_move(1, 0, 5, color ^ 1, QUEEN, PAWNS,
						from, to);
				u64 knight_prom = create_move(2, 0, 5, color ^ 1, QUEEN, PAWNS,
						from, to);
				u64 bishop_prom = create_move(3, 0, 5, color ^ 1, QUEEN, PAWNS,
						from, to);

				move_list[pos++] = queen_prom;
				move_list[pos++] = rook_prom;
				move_list[pos++] = knight_prom;
				move_list[pos++] = bishop_prom;
			} else if (piece_bb[color][BISHOPS]) {
				u64 bishops_bb = piece_bb[color][BISHOPS];
				while (bishops_bb) {
					const u8 sq = bit_scan_forward(bishops_bb);
					bishops_bb &= bishops_bb - 1;

					if (sq == to) {
						u64 queen_prom = create_move(0, 0, 5, color ^ 1,
								BISHOPS, PAWNS, from, to);
						u64 rook_prom = create_move(1, 0, 5, color ^ 1, BISHOPS,
								PAWNS, from, to);
						u64 knight_prom = create_move(2, 0, 5, color ^ 1,
								BISHOPS, PAWNS, from, to);
						u64 bishop_prom = create_move(3, 0, 5, color ^ 1,
								BISHOPS, PAWNS, from, to);

						move_list[pos++] = queen_prom;
						move_list[pos++] = rook_prom;
						move_list[pos++] = knight_prom;
						move_list[pos++] = bishop_prom;
					}
				}
			} else if (piece_bb[color][KNIGHTS]) {
				u64 knights_bb = piece_bb[color][KNIGHTS];
				while (knights_bb) {
					const u8 sq = bit_scan_forward(knights_bb);
					knights_bb &= knights_bb - 1;

					if (sq == to) {
						u64 queen_prom = create_move(0, 0, 5, color ^ 1,
								KNIGHTS, PAWNS, from, to);
						u64 rook_prom = create_move(1, 0, 5, color ^ 1, KNIGHTS,
								PAWNS, from, to);
						u64 knight_prom = create_move(2, 0, 5, color ^ 1,
								KNIGHTS, PAWNS, from, to);
						u64 bishop_prom = create_move(3, 0, 5, color ^ 1,
								KNIGHTS, PAWNS, from, to);

						move_list[pos++] = queen_prom;
						move_list[pos++] = rook_prom;
						move_list[pos++] = knight_prom;
						move_list[pos++] = bishop_prom;
					}
				}
			} else if (piece_bb[color][ROOKS]) {
				u64 rooks_bb = piece_bb[color][ROOKS];
				while (rooks_bb) {
					const u8 sq = bit_scan_forward(rooks_bb);
					rooks_bb &= rooks_bb - 1;

					if (sq == to) {
						u64 queen_prom = create_move(0, 0, 5, color ^ 1, ROOKS,
								PAWNS, from, to);
						u64 rook_prom = create_move(1, 0, 5, color ^ 1, ROOKS,
								PAWNS, from, to);
						u64 knight_prom = create_move(2, 0, 5, color ^ 1, ROOKS,
								PAWNS, from, to);
						u64 bishop_prom = create_move(3, 0, 5, color ^ 1, ROOKS,
								PAWNS, from, to);

						move_list[pos++] = queen_prom;
						move_list[pos++] = rook_prom;
						move_list[pos++] = knight_prom;
						move_list[pos++] = bishop_prom;
					}
				}
			}
		}

		u8 to_push = (from << 8) >> (16 * (color ^ 1)) & empty;

		while (to_push) {
			const u8 to = bit_scan_forward(to_push);
			to_push &= to_push - 1;

			u64 queen_prom = create_move(0, 0, 5, color ^ 1, DUMMY, PAWNS, from,
					to);
			u64 rook_prom = create_move(1, 0, 5, color ^ 1, DUMMY, PAWNS, from,
					to);
			u64 knight_prom = create_move(2, 0, 5, color ^ 1, DUMMY, PAWNS,
					from, to);
			u64 bishop_prom = create_move(3, 0, 5, color ^ 1, DUMMY, PAWNS,
					from, to);

			move_list[pos++] = queen_prom;
			move_list[pos++] = rook_prom;
			move_list[pos++] = knight_prom;
			move_list[pos++] = bishop_prom;
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

u32 create_move(u8 promotion_type, u8 castle_dir, u8 move_type, u8 color,
		u8 c_piece, u8 piece, u8 from, u8 to) {

	return (0ull | (u32) promotion_type << 24 | (u32) castle_dir << 22
			| (u32) move_type << 19 | (u32) color << 18 | (u32) c_piece << 15
			| (u32) piece << 12 | (u32) from << 6 | (u32) to);
}

/* Extract data from a move structure */
#define prom_type(move)   		(move & 0x3000000) >> 24
#define castle_dir(move)		(move & 0xC00000) >> 22
#define move_type(move)         (move & 0x380000) >> 19
#define color_type(move)        (move & 0x40000) >> 18
#define c_piece_type(move)      (move & 0x38000) >> 15
#define piece_type(move)	    (move & 0x7000) >> 12  	
#define from_sq(move)          	(move & 0xFC0) >> 6
#define to_sq(move)				move & 0x3F

