#include <stdbool.h>
#include "make_unmake.h"
#include "utility.h"
#include "Globals.h"

void make_move(u32 move) {

	u8 fromSq = from_sq(move);
	u8 toSq = to_sq(move);

	u64 from_bb, to_bb, from_to_bb, piece, c_piece;

	from_bb = index_bb[from_sq(move)];
	to_bb = index_bb[to_sq(move)];

	from_to_bb = from_bb ^ to_bb;

	piece = pieceType(move);
	c_piece = cPieceType(move);
	color = colorType(move);

	moveStack[ply].epFlag = 0;

	switch (move_type(move)) {

	case 0:
		quiet++;

		pieceBB[color][piece] ^= from_to_bb;
		pieceBB[color][PIECES] ^= from_to_bb;
		occupied ^= from_to_bb;
		empty ^= from_to_bb;

		moveStack[ply].prevCastleFlags = moveStack[ply].castleFlags;

		// update castle flags
		if (piece == KING) {
			if (color == WHITE) {
				moveStack[ply].castleFlags &= ~(CastleFlagWhiteKing
						| CastleFlagWhiteQueen);
			} else {
				moveStack[ply].castleFlags &= ~(CastleFlagBlackKing
						| CastleFlagBlackQueen);
			}
		} else if (piece == ROOKS) {
			moveStack[ply].castleFlags &= rookCastleFlagMask[fromSq];
		}

		break;
	case 1:
		cap++;

		pieceBB[color][piece] ^= from_to_bb;
		pieceBB[color][PIECES] ^= from_to_bb;
		pieceBB[color ^ 1][c_piece] ^= to_bb;
		pieceBB[color ^ 1][PIECES] ^= to_bb;

		occupied ^= from_bb;
		empty ^= from_bb;

		moveStack[ply].prevCastleFlags = moveStack[ply].castleFlags;

		// update castle flags
		if (piece == KING) {
			if (color == WHITE) {
				moveStack[ply].castleFlags &= ~(CastleFlagWhiteKing
						| CastleFlagWhiteQueen);
			} else {
				moveStack[ply].castleFlags &= ~(CastleFlagBlackKing
						| CastleFlagBlackQueen);
			}
		} else if (piece == ROOKS) {
			moveStack[ply].castleFlags &= rookCastleFlagMask[fromSq];
		}

		if(c_piece == ROOKS) {
			moveStack[ply].castleFlags &= rookCastleFlagMask[toSq];
		}

		break;

	case 2:
		quiet++;
		// pawn double pushes

		moveStack[ply].epFlag = 1;
		moveStack[ply].epSquare = (from_bb << 8) >> 16 * color;

		pieceBB[color][piece] ^= from_to_bb;
		pieceBB[color][PIECES] ^= from_to_bb;

		occupied ^= from_to_bb;
		empty ^= from_to_bb;

		break;

	case 3:
		ep++;
		//  en_passant capture

		pieceBB[color][PAWNS] ^= from_to_bb;
		pieceBB[color][PIECES] ^= from_to_bb;

		if (color == WHITE) {

			pieceBB[BLACK][PAWNS] ^= (to_bb >> 8);
			pieceBB[BLACK][PIECES] ^= (to_bb >> 8);

			// remove the original capturing piece
			occupied ^= from_bb;
			// add the capturing piece
			occupied ^= to_bb;
			// remove the captured piece
			occupied ^= (to_bb >> 8);
		} else {

			pieceBB[WHITE][PAWNS] ^= (to_bb << 8);
			pieceBB[WHITE][PIECES] ^= (to_bb << 8);

			// remove the original capturing piece
			occupied ^= from_bb;
			// add the capturing piece
			occupied ^= to_bb;
			// remove the captured piece
			occupied ^= (to_bb << 8);
		}

		empty = ~occupied;

		break;

	case 4:
		cas++;

		u8 castleDirection = castleDir(move);

		moveStack[ply].prevCastleFlags = moveStack[ply].castleFlags;

		if (color == WHITE) {
			moveStack[ply].castleFlags &= ~(CastleFlagWhiteKing
					| CastleFlagWhiteQueen);

			if (castleDirection == WHITE_CASTLE_QUEEN_SIDE) {

				//clear out king and rook
				pieceBB[WHITE][piece] ^= 0x0000000000000010U;
				pieceBB[WHITE][c_piece] ^= 0x0000000000000001U;

				// set king and rook
				pieceBB[WHITE][piece] ^= 0x0000000000000004U;
				pieceBB[WHITE][c_piece] ^= 0x0000000000000008U;

				// update pieces
				pieceBB[WHITE][PIECES] ^= 0x0000000000000011U;
				pieceBB[WHITE][PIECES] ^= 0x000000000000000CU;
			} else if (castleDirection == WHITE_CASTLE_KING_SIDE) {

				//clear out king and rook
				pieceBB[WHITE][piece] ^= 0x0000000000000010U;
				pieceBB[WHITE][c_piece] ^= 0x0000000000000080U;

				// set king and rook
				pieceBB[WHITE][piece] ^= 0x0000000000000040U;
				pieceBB[WHITE][c_piece] ^= 0x0000000000000020U;

				// update pieces
				pieceBB[WHITE][PIECES] ^= 0x0000000000000090U;
				pieceBB[WHITE][PIECES] ^= 0x0000000000000060U;
			}
		} else {

			moveStack[ply].castleFlags &= ~(CastleFlagBlackKing
					| CastleFlagBlackQueen);

			if (castleDirection == BLACK_CASTLE_QUEEN_SIDE) {

				//clear out king and rook
				pieceBB[BLACK][piece] ^= 0x1000000000000000U;
				pieceBB[BLACK][c_piece] ^= 0x0100000000000000U;

				// set king and rook
				pieceBB[BLACK][piece] ^= 0x0400000000000000U;
				pieceBB[BLACK][c_piece] ^= 0x0800000000000000U;

				// update pieces
				pieceBB[BLACK][PIECES] ^= 0x1100000000000000U;
				pieceBB[BLACK][PIECES] ^= 0x0C00000000000000U;

			} else if (castleDirection == BLACK_CASTLE_KING_SIDE) {

				//clear out king and rook
				pieceBB[BLACK][piece] ^= 0x1000000000000000U;
				pieceBB[BLACK][c_piece] ^= 0x8000000000000000U;

				// set king and rook
				pieceBB[BLACK][piece] ^= 0x4000000000000000U;
				pieceBB[BLACK][c_piece] ^= 0x2000000000000000U;

				// update pieces
				pieceBB[BLACK][PIECES] ^= 0x9000000000000000U;
				pieceBB[BLACK][PIECES] ^= 0x6000000000000000U;
			}
		}

		occupied = pieceBB[WHITE][PIECES] | pieceBB[BLACK][PIECES];
		empty = ~occupied;

		break;

	case MOVE_TYPE_PROMOTION:

		prom++;

		switch (promType(move)) {

		case PROMOTE_TO_QUEEN:
			pieceBB[color][PAWNS] ^= from_bb;
			pieceBB[color][QUEEN] ^= to_bb;
			pieceBB[color][PIECES] ^= from_to_bb;

			if (c_piece != DUMMY) {
				pieceBB[color ^ 1][c_piece] ^= to_bb;
				pieceBB[color ^ 1][PIECES] ^= to_bb;

				occupied ^= from_bb;
				empty ^= from_bb;

			} else {

				occupied ^= from_to_bb;
				empty ^= from_to_bb;
			}

			break;

		case PROMOTE_TO_ROOK:

			pieceBB[color][PAWNS] ^= from_bb;
			pieceBB[color][ROOKS] ^= to_bb;
			pieceBB[color][PIECES] ^= from_to_bb;

			if (c_piece != DUMMY) {
				pieceBB[color ^ 1][c_piece] ^= to_bb;
				pieceBB[color ^ 1][PIECES] ^= to_bb;

				occupied ^= from_bb;
				empty ^= from_bb;

			} else {
				occupied ^= from_to_bb;
				empty ^= from_to_bb;
			}

			break;

		case PROMOTE_TO_KNIGHT:
			pieceBB[color][PAWNS] ^= from_bb;
			pieceBB[color][KNIGHTS] ^= to_bb;
			pieceBB[color][PIECES] ^= from_to_bb;

			if (c_piece != DUMMY) {
				pieceBB[color ^ 1][c_piece] ^= to_bb;
				pieceBB[color ^ 1][PIECES] ^= to_bb;

				occupied ^= from_bb;
				empty ^= from_bb;

			} else {
				occupied ^= from_to_bb;
				empty ^= from_to_bb;
			}

			break;

		case PROMOTE_TO_BISHOP:
			pieceBB[color][PAWNS] ^= from_bb;
			pieceBB[color][BISHOPS] ^= to_bb;
			pieceBB[color][PIECES] ^= from_to_bb;

			if (c_piece != DUMMY) {
				pieceBB[color ^ 1][c_piece] ^= to_bb;
				pieceBB[color ^ 1][PIECES] ^= to_bb;

				occupied ^= from_bb;
				empty ^= from_bb;

			} else {
				occupied ^= from_to_bb;
				empty ^= from_to_bb;
			}
			break;
		}

		if(c_piece == ROOKS) {
			moveStack[ply].prevCastleFlags = moveStack[ply].castleFlags;
			moveStack[ply].castleFlags &= rookCastleFlagMask[toSq];
		}

		break;
		default:
			break;
	}
}

void unmake_move(u32 move) {
	u64 from_bb, to_bb, from_to_bb, piece, c_piece, color;

	u8 castleDirection = castleDir(move);

	from_bb = index_bb[from_sq(move)];
	to_bb = index_bb[to_sq(move)];

	from_to_bb = from_bb ^ to_bb;

	piece = pieceType(move);
	c_piece = cPieceType(move);
	color = colorType(move);

	moveStack[ply].castleFlags = moveStack[ply].prevCastleFlags;

	switch (move_type(move)) {

	case 0:

		pieceBB[color][piece] ^= from_to_bb;
		pieceBB[color][PIECES] ^= from_to_bb;

		occupied ^= from_to_bb;
		empty ^= from_to_bb;

		break;
	case 1:

		pieceBB[color][piece] ^= from_to_bb;
		pieceBB[color][PIECES] ^= from_to_bb;

		pieceBB[color ^ 1][c_piece] ^= to_bb;
		pieceBB[color ^ 1][PIECES] ^= to_bb;

		occupied ^= from_bb;
		empty ^= from_bb;

		break;
	case 2:

		pieceBB[color][piece] ^= from_to_bb;
		pieceBB[color][PIECES] ^= from_to_bb;

		occupied ^= from_to_bb;
		empty ^= from_to_bb;

		break;

	case 3:

		pieceBB[color][PAWNS] ^= from_to_bb;
		pieceBB[color][PIECES] ^= from_to_bb;

		if (color == WHITE) {

			pieceBB[BLACK][PAWNS] ^= (to_bb >> 8);
			pieceBB[BLACK][PIECES] ^= (to_bb >> 8);

			// restore the capturing piece
			occupied ^= from_bb;
			// remove the capturing piece
			occupied ^= to_bb;
			// restore the captured piece
			occupied ^= (to_bb >> 8);
		} else {

			pieceBB[WHITE][PAWNS] ^= (to_bb << 8);
			pieceBB[WHITE][PIECES] ^= (to_bb << 8);

			// restore the capturing piece
			occupied ^= from_bb;
			// remove the capturing piece
			occupied ^= to_bb;
			// restore the captured piece
			occupied ^= (to_bb << 8);
		}

		empty = ~occupied;

		break;
	case 4:

		if (castleDirection == WHITE_CASTLE_QUEEN_SIDE) {

			// clear king and rook
			pieceBB[WHITE][piece] ^= 0x0000000000000004U;
			pieceBB[WHITE][c_piece] ^= 0x0000000000000008U;

			//set king and rook
			pieceBB[WHITE][piece] ^= 0x0000000000000010U;
			pieceBB[WHITE][c_piece] ^= 0x0000000000000001U;

			// update pieces
			pieceBB[WHITE][PIECES] ^= 0x000000000000000CU;
			pieceBB[WHITE][PIECES] ^= 0x0000000000000011U;
		} else if (castleDirection == WHITE_CASTLE_KING_SIDE) {

			pieceBB[WHITE][piece] ^= 0x0000000000000040U;
			pieceBB[WHITE][c_piece] ^= 0x0000000000000020U;

			pieceBB[WHITE][piece] ^= 0x0000000000000010U;
			pieceBB[WHITE][c_piece] ^= 0x0000000000000080U;

			pieceBB[WHITE][PIECES] ^= 0x0000000000000060U;
			pieceBB[WHITE][PIECES] ^= 0x0000000000000090U;
		} else if (castleDirection == BLACK_CASTLE_QUEEN_SIDE) {

			pieceBB[BLACK][piece] ^= 0x0400000000000000U;
			pieceBB[BLACK][c_piece] ^= 0x0800000000000000U;

			pieceBB[BLACK][piece] ^= 0x1000000000000000U;
			pieceBB[BLACK][c_piece] ^= 0x0100000000000000U;

			pieceBB[BLACK][PIECES] ^= 0x0C00000000000000U;
			pieceBB[BLACK][PIECES] ^= 0x1100000000000000U;

		} else if (castleDirection == BLACK_CASTLE_KING_SIDE) {

			pieceBB[BLACK][piece] ^= 0x4000000000000000U;
			pieceBB[BLACK][c_piece] ^= 0x2000000000000000U;

			pieceBB[BLACK][piece] ^= 0x1000000000000000U;
			pieceBB[BLACK][c_piece] ^= 0x8000000000000000U;

			pieceBB[BLACK][PIECES] ^= 0x6000000000000000U;
			pieceBB[BLACK][PIECES] ^= 0x9000000000000000U;
		}

		occupied = pieceBB[WHITE][PIECES] | pieceBB[BLACK][PIECES];
		empty = ~occupied;

		break;
	case MOVE_TYPE_PROMOTION:

		switch (promType(move)) {

		case PROMOTE_TO_QUEEN:

			pieceBB[color][PAWNS] ^= from_bb;
			pieceBB[color][QUEEN] ^= to_bb;
			pieceBB[color][PIECES] ^= from_to_bb;

			if (c_piece != DUMMY) {
				pieceBB[color ^ 1][c_piece] ^= to_bb;
				pieceBB[color ^ 1][PIECES] ^= to_bb;

				occupied ^= from_bb;
				empty ^= from_bb;
			} else {
				occupied ^= from_to_bb;
				empty ^= from_to_bb;
			}

			break;
		case PROMOTE_TO_ROOK:
			pieceBB[color][PAWNS] ^= from_bb;
			pieceBB[color][ROOKS] ^= to_bb;
			pieceBB[color][PIECES] ^= from_to_bb;

			if (c_piece != DUMMY) {
				pieceBB[color ^ 1][c_piece] ^= to_bb;
				pieceBB[color ^ 1][PIECES] ^= to_bb;

				occupied ^= from_bb;
				empty ^= from_bb;

			} else {
				occupied ^= from_to_bb;
				empty ^= from_to_bb;
			}

			break;
		case PROMOTE_TO_KNIGHT:
			pieceBB[color][PAWNS] ^= from_bb;
			pieceBB[color][KNIGHTS] ^= to_bb;
			pieceBB[color][PIECES] ^= from_to_bb;

			if (c_piece != DUMMY) {
				pieceBB[color ^ 1][c_piece] ^= to_bb;
				pieceBB[color ^ 1][PIECES] ^= to_bb;

				occupied ^= from_bb;
				empty ^= from_bb;

			} else {
				occupied ^= from_to_bb;
				empty ^= from_to_bb;
			}

			break;

		case PROMOTE_TO_BISHOP:
			pieceBB[color][PAWNS] ^= from_bb;
			pieceBB[color][BISHOPS] ^= to_bb;
			pieceBB[color][PIECES] ^= from_to_bb;

			if (c_piece != DUMMY) {
				pieceBB[color ^ 1][c_piece] ^= to_bb;
				pieceBB[color ^ 1][PIECES] ^= to_bb;

				occupied ^= from_bb;
				empty ^= from_bb;

			} else {
				occupied ^= from_to_bb;
				empty ^= from_to_bb;
			}
			break;
		}

		break;
	}
}
