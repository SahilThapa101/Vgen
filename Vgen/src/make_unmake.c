//
//  make_unmake.c
//  Vgen
//
//  Created by Amar Thapa on 23/12/18.
//  Copyright © 2018 Amar Thapa. All rights reserved.
//

#include <stdbool.h>
#include "make_unmake.h"
#include "utility.h"
#include "hash.h"

void make_move(u32 move) {

	u8 fromSq = from_sq(move);
	u8 toSq = to_sq(move);

	u64 from_bb, to_bb, from_to_bb, piece, c_piece;

	from_bb = index_bb[from_sq(move)];
	to_bb = index_bb[to_sq(move)];

	from_to_bb = from_bb ^ to_bb;

	piece = pieceType(move);
	c_piece = cPieceType(move);

	u8 sideToMove = colorType(move);

	moveStack[ply].epFlag = 0;
	moveStack[ply].prevCastleFlags = moveStack[ply].castleFlags;

	tempFiftyMoves = fiftyMoves;
	switch (move_type(move)) {

	case MOVE_NORMAL:
		quiet++;
		fiftyMoves++;

		pieceBB[sideToMove][piece] ^= from_to_bb;
		pieceBB[sideToMove][PIECES] ^= from_to_bb;
		occupied ^= from_to_bb;
		empty ^= from_to_bb;

		hashKey ^= zobrist[sideToMove][piece][fromSq] ^ zobrist[sideToMove][piece][toSq];

		if (piece == KING && (fromSq == 4 || fromSq == 60)) {
			if (sideToMove == WHITE) {

				int castleQueenSide = moveStack[ply].castleFlags & CastleFlagWhiteQueen;
				int castleKingSide = moveStack[ply].castleFlags & CastleFlagWhiteKing;

				if (castleQueenSide) {
					moveStack[ply].castleFlags &= ~CastleFlagWhiteQueen;
					hashKey ^= KEY_FLAG_WHITE_CASTLE_QUEEN_SIDE;
				}

				if (castleKingSide) {
					moveStack[ply].castleFlags &= ~CastleFlagWhiteKing;
					hashKey ^= KEY_FLAG_WHITE_CASTLE_KING_SIDE;
				}
			} else {

				int castleQueenSide = moveStack[ply].castleFlags
						& CastleFlagBlackQueen;
				int castleKingSide = moveStack[ply].castleFlags
						& CastleFlagBlackKing;

				if (castleQueenSide) {
					moveStack[ply].castleFlags &= ~CastleFlagBlackQueen;
					hashKey ^= KEY_FLAG_BLACK_CASTLE_QUEEN_SIDE;
				}

				if (castleKingSide) {
					moveStack[ply].castleFlags &= ~CastleFlagBlackKing;
					hashKey ^= KEY_FLAG_BLACK_CASTLE_KING_SIDE;
				}
			}
		} else if (piece == ROOKS && (fromSq == 0 || fromSq == 7 || fromSq == 56 || fromSq == 63)) {

			if (fromSq == 0
					&& (moveStack[ply].castleFlags & CastleFlagWhiteQueen)) {

				hashKey ^= KEY_FLAG_WHITE_CASTLE_QUEEN_SIDE;
			} else if (fromSq == 56
					&& (moveStack[ply].castleFlags & CastleFlagBlackQueen)) {

				hashKey ^= KEY_FLAG_BLACK_CASTLE_QUEEN_SIDE;
			} else if (fromSq == 7
					&& (moveStack[ply].castleFlags & CastleFlagWhiteKing)) {

				hashKey ^= KEY_FLAG_WHITE_CASTLE_KING_SIDE;
			} else if (fromSq == 63
					&& (moveStack[ply].castleFlags & CastleFlagBlackKing)) {

				hashKey ^= KEY_FLAG_BLACK_CASTLE_KING_SIDE;
			}

			moveStack[ply].castleFlags &= rookCastleFlagMask[fromSq];
		}
		break;
	case MOVE_CAPTURE:
		cap++;
		fiftyMoves = 0;

		pieceBB[sideToMove][piece] ^= from_to_bb;
		pieceBB[sideToMove][PIECES] ^= from_to_bb;
		pieceBB[sideToMove ^ 1][c_piece] ^= to_bb;
		pieceBB[sideToMove ^ 1][PIECES] ^= to_bb;

		occupied ^= from_bb;
		empty ^= from_bb;

		hashKey ^= zobrist[sideToMove][piece][fromSq]
											  ^ zobrist[sideToMove][piece][toSq];
		hashKey ^= zobrist[sideToMove ^ 1][c_piece][toSq];

		// update castle flags
		if (piece == KING) {
			if (sideToMove == WHITE) {

				int castleQueenSide = moveStack[ply].castleFlags
						& CastleFlagWhiteQueen;
				int castleKingSide = moveStack[ply].castleFlags
						& CastleFlagWhiteKing;

				if (castleQueenSide) {
					moveStack[ply].castleFlags &= ~CastleFlagWhiteQueen;
					hashKey ^= KEY_FLAG_WHITE_CASTLE_QUEEN_SIDE;
				}

				if (castleKingSide) {
					moveStack[ply].castleFlags &= ~CastleFlagWhiteKing;
					hashKey ^= KEY_FLAG_WHITE_CASTLE_KING_SIDE;
				}
			} else {

				int castleQueenSide = moveStack[ply].castleFlags
						& CastleFlagBlackQueen;
				int castleKingSide = moveStack[ply].castleFlags
						& CastleFlagBlackKing;

				if (castleQueenSide) {
					moveStack[ply].castleFlags &= ~CastleFlagBlackQueen;
					hashKey ^= KEY_FLAG_BLACK_CASTLE_QUEEN_SIDE;
				}

				if (castleKingSide) {
					moveStack[ply].castleFlags &= ~CastleFlagBlackKing;
					hashKey ^= KEY_FLAG_BLACK_CASTLE_KING_SIDE;
				}
			}
		} else if (piece == ROOKS) {

			if (fromSq == 0
					&& (moveStack[ply].castleFlags & CastleFlagWhiteQueen)) {

				hashKey ^= KEY_FLAG_WHITE_CASTLE_QUEEN_SIDE;
			} else if (fromSq == 56
					&& (moveStack[ply].castleFlags & CastleFlagBlackQueen)) {

				hashKey ^= KEY_FLAG_BLACK_CASTLE_QUEEN_SIDE;
			} else if (fromSq == 7
					&& (moveStack[ply].castleFlags & CastleFlagWhiteKing)) {

				hashKey ^= KEY_FLAG_WHITE_CASTLE_KING_SIDE;
			} else if (fromSq == 63
					&& (moveStack[ply].castleFlags & CastleFlagBlackKing)) {

				hashKey ^= KEY_FLAG_BLACK_CASTLE_KING_SIDE;
			}

			moveStack[ply].castleFlags &= rookCastleFlagMask[fromSq];
		}

		if (c_piece == ROOKS) {

			if (toSq == 0
					&& (moveStack[ply].castleFlags & CastleFlagWhiteQueen)) {

				hashKey ^= KEY_FLAG_WHITE_CASTLE_QUEEN_SIDE;
			} else if (toSq == 56
					&& (moveStack[ply].castleFlags & CastleFlagBlackQueen)) {

				hashKey ^= KEY_FLAG_BLACK_CASTLE_QUEEN_SIDE;
			} else if (toSq == 7
					&& (moveStack[ply].castleFlags & CastleFlagWhiteKing)) {

				hashKey ^= KEY_FLAG_WHITE_CASTLE_KING_SIDE;
			} else if (toSq == 63
					&& (moveStack[ply].castleFlags & CastleFlagBlackKing)) {

				hashKey ^= KEY_FLAG_BLACK_CASTLE_KING_SIDE;
			}

			moveStack[ply].castleFlags &= rookCastleFlagMask[toSq];
		}

		break;

	case MOVE_DOUBLE_PUSH:

		quiet++;
		fiftyMoves++;
		// pawn double pushes

		u64 epSqBitboard = (from_bb << 8) >> (16 * sideToMove);

		moveStack[ply].epFlag = 1;
		moveStack[ply].epSquare = epSqBitboard;

		pieceBB[sideToMove][piece] ^= from_to_bb;
		pieceBB[sideToMove][PIECES] ^= from_to_bb;

		occupied ^= from_to_bb;
		empty ^= from_to_bb;

		hashKey ^= zobrist[sideToMove][piece][fromSq]
											  ^ zobrist[sideToMove][piece][toSq];

		if (epSqBitboard & A_FILE) {
			hashKey ^= KEY_EP_A_FILE;
		} else if (epSqBitboard & B_FILE) {

			hashKey ^= KEY_EP_B_FILE;
		} else if (epSqBitboard & C_FILE) {

			hashKey ^= KEY_EP_C_FILE;
		} else if (epSqBitboard & D_FILE) {

			hashKey ^= KEY_EP_D_FILE;
		} else if (epSqBitboard & E_FILE) {

			hashKey ^= KEY_EP_E_FILE;
		} else if (epSqBitboard & F_FILE) {

			hashKey ^= KEY_EP_F_FILE;
		} else if (epSqBitboard & G_FILE) {

			hashKey ^= KEY_EP_G_FILE;
		} else if (epSqBitboard & H_FILE) {

			hashKey ^= KEY_EP_H_FILE;
		}

		break;

	case MOVE_ENPASSANT:

		ep++;
		fiftyMoves = 0;
		//  en_passant capture

		pieceBB[sideToMove][PAWNS] ^= from_to_bb;
		pieceBB[sideToMove][PIECES] ^= from_to_bb;

		if (sideToMove == WHITE) {

			pieceBB[BLACK][PAWNS] ^= (to_bb >> 8);
			pieceBB[BLACK][PIECES] ^= (to_bb >> 8);

			// remove the original capturing piece
			occupied ^= from_bb;
			// add the capturing piece
			occupied ^= to_bb;
			// remove the captured piece
			occupied ^= (to_bb >> 8);

			hashKey ^= zobrist[sideToMove][PAWNS][fromSq]
												  ^ zobrist[sideToMove][piece][toSq];
			hashKey ^= zobrist[sideToMove ^ 1][PAWNS][toSq - 8];
		} else {

			pieceBB[WHITE][PAWNS] ^= (to_bb << 8);
			pieceBB[WHITE][PIECES] ^= (to_bb << 8);

			// remove the original capturing piece
			occupied ^= from_bb;
			// add the capturing piece
			occupied ^= to_bb;
			// remove the captured piece
			occupied ^= (to_bb << 8);

			hashKey ^= zobrist[sideToMove][PAWNS][fromSq]
												  ^ zobrist[sideToMove][piece][toSq];
			hashKey ^= zobrist[sideToMove ^ 1][PAWNS][toSq + 8];
		}

		empty = ~occupied;

		break;
	case MOVE_CASTLE:
		cas++;
		fiftyMoves++;

		u8 castleDirection = castleDir(move);

		if (sideToMove == WHITE) {
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

				hashKey ^= zobrist[WHITE][KING][4] ^ zobrist[WHITE][KING][2];
				hashKey ^= zobrist[WHITE][ROOKS][0] ^ zobrist[WHITE][ROOKS][3];
				hashKey ^= KEY_FLAG_WHITE_CASTLE_QUEEN_SIDE;
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

				hashKey ^= zobrist[WHITE][KING][4] ^ zobrist[WHITE][KING][6];
				hashKey ^= zobrist[WHITE][ROOKS][7] ^ zobrist[WHITE][ROOKS][5];
				hashKey ^= KEY_FLAG_WHITE_CASTLE_KING_SIDE;
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

				hashKey ^= zobrist[BLACK][KING][60] ^ zobrist[BLACK][KING][58];
				hashKey ^= zobrist[BLACK][ROOKS][56]
												 ^ zobrist[BLACK][ROOKS][59];
				hashKey ^= KEY_FLAG_BLACK_CASTLE_QUEEN_SIDE;
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

				hashKey ^= zobrist[BLACK][KING][60] ^ zobrist[BLACK][KING][62];
				hashKey ^= zobrist[BLACK][ROOKS][63]
												 ^ zobrist[BLACK][ROOKS][61];
				hashKey ^= KEY_FLAG_BLACK_CASTLE_KING_SIDE;
			}
		}

		occupied = pieceBB[WHITE][PIECES] | pieceBB[BLACK][PIECES];
		empty = ~occupied;

		break;

	case MOVE_PROMOTION:

		prom++;

		switch (promType(move)) {

		case PROMOTE_TO_QUEEN:
			pieceBB[sideToMove][PAWNS] ^= from_bb;
			pieceBB[sideToMove][QUEEN] ^= to_bb;
			pieceBB[sideToMove][PIECES] ^= from_to_bb;

			if (c_piece != DUMMY) {
				pieceBB[sideToMove ^ 1][c_piece] ^= to_bb;
				pieceBB[sideToMove ^ 1][PIECES] ^= to_bb;

				occupied ^= from_bb;
				empty ^= from_bb;

				fiftyMoves = 0;
			} else {

				occupied ^= from_to_bb;
				empty ^= from_to_bb;

				fiftyMoves++;
			}

			hashKey ^= zobrist[sideToMove][PAWNS][fromSq];
			hashKey ^= zobrist[sideToMove][QUEEN][toSq];

			break;

		case PROMOTE_TO_ROOK:

			pieceBB[sideToMove][PAWNS] ^= from_bb;
			pieceBB[sideToMove][ROOKS] ^= to_bb;
			pieceBB[sideToMove][PIECES] ^= from_to_bb;

			if (c_piece != DUMMY) {
				pieceBB[sideToMove ^ 1][c_piece] ^= to_bb;
				pieceBB[sideToMove ^ 1][PIECES] ^= to_bb;

				occupied ^= from_bb;
				empty ^= from_bb;

				fiftyMoves = 0;
			} else {
				occupied ^= from_to_bb;
				empty ^= from_to_bb;

				fiftyMoves++;
			}

			hashKey ^= zobrist[sideToMove][PAWNS][fromSq];
			hashKey ^= zobrist[sideToMove][ROOKS][toSq];

			break;

		case PROMOTE_TO_KNIGHT:
			pieceBB[sideToMove][PAWNS] ^= from_bb;
			pieceBB[sideToMove][KNIGHTS] ^= to_bb;
			pieceBB[sideToMove][PIECES] ^= from_to_bb;

			if (c_piece != DUMMY) {
				pieceBB[sideToMove ^ 1][c_piece] ^= to_bb;
				pieceBB[sideToMove ^ 1][PIECES] ^= to_bb;

				occupied ^= from_bb;
				empty ^= from_bb;

				fiftyMoves = 0;
			} else {
				occupied ^= from_to_bb;
				empty ^= from_to_bb;

				fiftyMoves++;
			}

			hashKey ^= zobrist[sideToMove][PAWNS][fromSq];
			hashKey ^= zobrist[sideToMove][KNIGHTS][toSq];

			break;

		case PROMOTE_TO_BISHOP:
			pieceBB[sideToMove][PAWNS] ^= from_bb;
			pieceBB[sideToMove][BISHOPS] ^= to_bb;
			pieceBB[sideToMove][PIECES] ^= from_to_bb;

			if (c_piece != DUMMY) {
				pieceBB[sideToMove ^ 1][c_piece] ^= to_bb;
				pieceBB[sideToMove ^ 1][PIECES] ^= to_bb;

				occupied ^= from_bb;
				empty ^= from_bb;

				fiftyMoves = 0;
			} else {
				occupied ^= from_to_bb;
				empty ^= from_to_bb;

				fiftyMoves++;
			}

			hashKey ^= zobrist[sideToMove][PAWNS][fromSq];
			hashKey ^= zobrist[sideToMove][BISHOPS][toSq];

			break;
		}

		// Todo update zobrist keys for castling
		if (c_piece == ROOKS) {
			moveStack[ply].prevCastleFlags = moveStack[ply].castleFlags;
			moveStack[ply].castleFlags &= rookCastleFlagMask[toSq];
		}

		break;

		default:
			break;
	}
}

void unmake_move(u32 move) {
	u64 from_bb, to_bb, from_to_bb, piece, c_piece;

	u8 castleDirection = castleDir(move);

	u8 fromSq = from_sq(move);
	u8 toSq = to_sq(move);

	from_bb = index_bb[fromSq];
	to_bb = index_bb[toSq];

	from_to_bb = from_bb ^ to_bb;

	piece = pieceType(move);
	c_piece = cPieceType(move);

	u8 sideToMove = colorType(move);

	moveStack[ply].castleFlags = moveStack[ply].prevCastleFlags;

	fiftyMoves = tempFiftyMoves;

	switch (move_type(move)) {

	case MOVE_NORMAL:

		pieceBB[sideToMove][piece] ^= from_to_bb;
		pieceBB[sideToMove][PIECES] ^= from_to_bb;

		occupied ^= from_to_bb;
		empty ^= from_to_bb;

		hashKey ^= zobrist[sideToMove][piece][fromSq]
											  ^ zobrist[sideToMove][piece][toSq];

		if (piece == KING) {
			if (sideToMove == WHITE) {

				int castleQueenSide = moveStack[ply].castleFlags
						& CastleFlagWhiteQueen;
				int castleKingSide = moveStack[ply].castleFlags
						& CastleFlagWhiteKing;

				if (castleQueenSide) {
					hashKey ^= KEY_FLAG_WHITE_CASTLE_QUEEN_SIDE;
				}

				if (castleKingSide) {
					hashKey ^= KEY_FLAG_WHITE_CASTLE_KING_SIDE;
				}
			} else {

				int castleQueenSide = moveStack[ply].castleFlags
						& CastleFlagBlackQueen;
				int castleKingSide = moveStack[ply].castleFlags
						& CastleFlagBlackKing;

				if (castleQueenSide) {
					hashKey ^= KEY_FLAG_BLACK_CASTLE_QUEEN_SIDE;
				}

				if (castleKingSide) {
					hashKey ^= KEY_FLAG_BLACK_CASTLE_KING_SIDE;
				}
			}
		} else if (piece == ROOKS) {

			if (fromSq == 0
					&& (moveStack[ply].castleFlags & CastleFlagWhiteQueen)) {

				hashKey ^= KEY_FLAG_WHITE_CASTLE_QUEEN_SIDE;
			} else if (fromSq == 56
					&& (moveStack[ply].castleFlags & CastleFlagBlackQueen)) {

				hashKey ^= KEY_FLAG_BLACK_CASTLE_QUEEN_SIDE;
			} else if (fromSq == 7
					&& (moveStack[ply].castleFlags & CastleFlagWhiteKing)) {

				hashKey ^= KEY_FLAG_WHITE_CASTLE_KING_SIDE;
			} else if (fromSq == 63
					&& (moveStack[ply].castleFlags & CastleFlagBlackKing)) {

				hashKey ^= KEY_FLAG_BLACK_CASTLE_KING_SIDE;
			}
		}
		break;
	case MOVE_CAPTURE:

		pieceBB[sideToMove][piece] ^= from_to_bb;
		pieceBB[sideToMove][PIECES] ^= from_to_bb;

		pieceBB[sideToMove ^ 1][c_piece] ^= to_bb;
		pieceBB[sideToMove ^ 1][PIECES] ^= to_bb;

		occupied ^= from_bb;
		empty ^= from_bb;

		hashKey ^= zobrist[sideToMove][piece][fromSq] ^ zobrist[sideToMove][piece][toSq];
		hashKey ^= zobrist[sideToMove ^ 1][c_piece][toSq];

		if (piece == KING) {
			if (sideToMove == WHITE) {

				int castleQueenSide = moveStack[ply].castleFlags
						& CastleFlagWhiteQueen;
				int castleKingSide = moveStack[ply].castleFlags
						& CastleFlagWhiteKing;

				if (castleQueenSide) {
					hashKey ^= KEY_FLAG_WHITE_CASTLE_QUEEN_SIDE;
				}

				if (castleKingSide) {
					hashKey ^= KEY_FLAG_WHITE_CASTLE_KING_SIDE;
				}
			} else {

				int castleQueenSide = moveStack[ply].castleFlags
						& CastleFlagBlackQueen;
				int castleKingSide = moveStack[ply].castleFlags
						& CastleFlagBlackKing;

				if (castleQueenSide) {
					hashKey ^= KEY_FLAG_BLACK_CASTLE_QUEEN_SIDE;
				}

				if (castleKingSide) {
					hashKey ^= KEY_FLAG_BLACK_CASTLE_KING_SIDE;
				}
			}
		} else if (piece == ROOKS) {

			if (fromSq == 0
					&& (moveStack[ply].castleFlags & CastleFlagWhiteQueen)) {

				hashKey ^= KEY_FLAG_WHITE_CASTLE_QUEEN_SIDE;
			} else if (fromSq == 56
					&& (moveStack[ply].castleFlags & CastleFlagBlackQueen)) {

				hashKey ^= KEY_FLAG_BLACK_CASTLE_QUEEN_SIDE;
			} else if (fromSq == 7
					&& (moveStack[ply].castleFlags & CastleFlagWhiteKing)) {

				hashKey ^= KEY_FLAG_WHITE_CASTLE_KING_SIDE;
			} else if (fromSq == 63
					&& (moveStack[ply].castleFlags & CastleFlagBlackKing)) {

				hashKey ^= KEY_FLAG_BLACK_CASTLE_KING_SIDE;
			}
		}

		if (c_piece == ROOKS) {

			if (toSq == 0
					&& (moveStack[ply].castleFlags & CastleFlagWhiteQueen)) {

				hashKey ^= KEY_FLAG_WHITE_CASTLE_QUEEN_SIDE;
			} else if (toSq == 56
					&& (moveStack[ply].castleFlags & CastleFlagBlackQueen)) {

				hashKey ^= KEY_FLAG_BLACK_CASTLE_QUEEN_SIDE;
			} else if (toSq == 7
					&& (moveStack[ply].castleFlags & CastleFlagWhiteKing)) {

				hashKey ^= KEY_FLAG_WHITE_CASTLE_KING_SIDE;
			} else if (toSq == 63
					&& (moveStack[ply].castleFlags & CastleFlagBlackKing)) {

				hashKey ^= KEY_FLAG_BLACK_CASTLE_KING_SIDE;
			}
		}

		break;
	case MOVE_DOUBLE_PUSH:

		pieceBB[sideToMove][piece] ^= from_to_bb;
		pieceBB[sideToMove][PIECES] ^= from_to_bb;

		occupied ^= from_to_bb;
		empty ^= from_to_bb;

		hashKey ^= zobrist[sideToMove][piece][fromSq]
											  ^ zobrist[sideToMove][piece][toSq];

		u64 epSqBitboard = (from_bb << 8) >> (16 * sideToMove);

		if (epSqBitboard & A_FILE) {
			hashKey ^= KEY_EP_A_FILE;
		} else if (epSqBitboard & B_FILE) {

			hashKey ^= KEY_EP_B_FILE;
		} else if (epSqBitboard & C_FILE) {

			hashKey ^= KEY_EP_C_FILE;
		} else if (epSqBitboard & D_FILE) {

			hashKey ^= KEY_EP_D_FILE;
		} else if (epSqBitboard & E_FILE) {

			hashKey ^= KEY_EP_E_FILE;
		} else if (epSqBitboard & F_FILE) {

			hashKey ^= KEY_EP_F_FILE;
		} else if (epSqBitboard & G_FILE) {

			hashKey ^= KEY_EP_G_FILE;
		} else if (epSqBitboard & H_FILE) {

			hashKey ^= KEY_EP_H_FILE;
		}

		break;

	case MOVE_ENPASSANT:

		pieceBB[sideToMove][PAWNS] ^= from_to_bb;
		pieceBB[sideToMove][PIECES] ^= from_to_bb;

		if (sideToMove == WHITE) {

			pieceBB[BLACK][PAWNS] ^= (to_bb >> 8);
			pieceBB[BLACK][PIECES] ^= (to_bb >> 8);

			// restore the capturing piece
			occupied ^= from_bb;
			// remove the capturing piece
			occupied ^= to_bb;
			// restore the captured piece
			occupied ^= (to_bb >> 8);

			hashKey ^= zobrist[sideToMove][PAWNS][fromSq]
												  ^ zobrist[sideToMove][piece][toSq];
			hashKey ^= zobrist[sideToMove ^ 1][PAWNS][toSq - 8];
		} else {

			pieceBB[WHITE][PAWNS] ^= (to_bb << 8);
			pieceBB[WHITE][PIECES] ^= (to_bb << 8);

			// restore the capturing piece
			occupied ^= from_bb;
			// remove the capturing piece
			occupied ^= to_bb;
			// restore the captured piece
			occupied ^= (to_bb << 8);

			hashKey ^= zobrist[sideToMove][PAWNS][fromSq]
												  ^ zobrist[sideToMove][piece][toSq];
			hashKey ^= zobrist[sideToMove ^ 1][PAWNS][toSq + 8];
		}

		empty = ~occupied;

		break;
	case MOVE_CASTLE:

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

			hashKey ^= zobrist[WHITE][KING][4] ^ zobrist[WHITE][KING][2];
			hashKey ^= zobrist[WHITE][ROOKS][0] ^ zobrist[WHITE][ROOKS][3];
			hashKey ^= KEY_FLAG_WHITE_CASTLE_QUEEN_SIDE;
		} else if (castleDirection == WHITE_CASTLE_KING_SIDE) {

			pieceBB[WHITE][piece] ^= 0x0000000000000040U;
			pieceBB[WHITE][c_piece] ^= 0x0000000000000020U;

			pieceBB[WHITE][piece] ^= 0x0000000000000010U;
			pieceBB[WHITE][c_piece] ^= 0x0000000000000080U;

			pieceBB[WHITE][PIECES] ^= 0x0000000000000060U;
			pieceBB[WHITE][PIECES] ^= 0x0000000000000090U;

			hashKey ^= zobrist[WHITE][KING][4] ^ zobrist[WHITE][KING][6];
			hashKey ^= zobrist[WHITE][ROOKS][7] ^ zobrist[WHITE][ROOKS][5];
			hashKey ^= KEY_FLAG_WHITE_CASTLE_KING_SIDE;
		} else if (castleDirection == BLACK_CASTLE_QUEEN_SIDE) {

			pieceBB[BLACK][piece] ^= 0x0400000000000000U;
			pieceBB[BLACK][c_piece] ^= 0x0800000000000000U;

			pieceBB[BLACK][piece] ^= 0x1000000000000000U;
			pieceBB[BLACK][c_piece] ^= 0x0100000000000000U;

			pieceBB[BLACK][PIECES] ^= 0x0C00000000000000U;
			pieceBB[BLACK][PIECES] ^= 0x1100000000000000U;

			hashKey ^= zobrist[BLACK][KING][60] ^ zobrist[BLACK][KING][58];
			hashKey ^= zobrist[BLACK][ROOKS][56] ^ zobrist[BLACK][ROOKS][59];
			hashKey ^= KEY_FLAG_BLACK_CASTLE_QUEEN_SIDE;
		} else if (castleDirection == BLACK_CASTLE_KING_SIDE) {

			pieceBB[BLACK][piece] ^= 0x4000000000000000U;
			pieceBB[BLACK][c_piece] ^= 0x2000000000000000U;

			pieceBB[BLACK][piece] ^= 0x1000000000000000U;
			pieceBB[BLACK][c_piece] ^= 0x8000000000000000U;

			pieceBB[BLACK][PIECES] ^= 0x6000000000000000U;
			pieceBB[BLACK][PIECES] ^= 0x9000000000000000U;

			hashKey ^= zobrist[BLACK][KING][60] ^ zobrist[BLACK][KING][62];
			hashKey ^= zobrist[BLACK][ROOKS][63] ^ zobrist[BLACK][ROOKS][61];
			hashKey ^= KEY_FLAG_BLACK_CASTLE_KING_SIDE;
		}

		occupied = pieceBB[WHITE][PIECES] | pieceBB[BLACK][PIECES];
		empty = ~occupied;

		break;
	case MOVE_PROMOTION:

		switch (promType(move)) {

		case PROMOTE_TO_QUEEN:

			pieceBB[sideToMove][PAWNS] ^= from_bb;
			pieceBB[sideToMove][QUEEN] ^= to_bb;
			pieceBB[sideToMove][PIECES] ^= from_to_bb;

			if (c_piece != DUMMY) {
				pieceBB[sideToMove ^ 1][c_piece] ^= to_bb;
				pieceBB[sideToMove ^ 1][PIECES] ^= to_bb;

				occupied ^= from_bb;
				empty ^= from_bb;
			} else {
				occupied ^= from_to_bb;
				empty ^= from_to_bb;
			}

			hashKey ^= zobrist[sideToMove][PAWNS][fromSq];
			hashKey ^= zobrist[sideToMove][QUEEN][toSq];

			break;
		case PROMOTE_TO_ROOK:
			pieceBB[sideToMove][PAWNS] ^= from_bb;
			pieceBB[sideToMove][ROOKS] ^= to_bb;
			pieceBB[sideToMove][PIECES] ^= from_to_bb;

			if (c_piece != DUMMY) {
				pieceBB[sideToMove ^ 1][c_piece] ^= to_bb;
				pieceBB[sideToMove ^ 1][PIECES] ^= to_bb;

				occupied ^= from_bb;
				empty ^= from_bb;

			} else {
				occupied ^= from_to_bb;
				empty ^= from_to_bb;
			}

			hashKey ^= zobrist[sideToMove][PAWNS][fromSq];
			hashKey ^= zobrist[sideToMove][ROOKS][toSq];

			break;
		case PROMOTE_TO_KNIGHT:
			pieceBB[sideToMove][PAWNS] ^= from_bb;
			pieceBB[sideToMove][KNIGHTS] ^= to_bb;
			pieceBB[sideToMove][PIECES] ^= from_to_bb;

			if (c_piece != DUMMY) {
				pieceBB[sideToMove ^ 1][c_piece] ^= to_bb;
				pieceBB[sideToMove ^ 1][PIECES] ^= to_bb;

				occupied ^= from_bb;
				empty ^= from_bb;

			} else {
				occupied ^= from_to_bb;
				empty ^= from_to_bb;
			}

			hashKey ^= zobrist[sideToMove][PAWNS][fromSq];
			hashKey ^= zobrist[sideToMove][KNIGHTS][toSq];

			break;

		case PROMOTE_TO_BISHOP:
			pieceBB[sideToMove][PAWNS] ^= from_bb;
			pieceBB[sideToMove][BISHOPS] ^= to_bb;
			pieceBB[sideToMove][PIECES] ^= from_to_bb;

			if (c_piece != DUMMY) {
				pieceBB[sideToMove ^ 1][c_piece] ^= to_bb;
				pieceBB[sideToMove ^ 1][PIECES] ^= to_bb;

				occupied ^= from_bb;
				empty ^= from_bb;

			} else {
				occupied ^= from_to_bb;
				empty ^= from_to_bb;
			}

			hashKey ^= zobrist[sideToMove][PAWNS][fromSq];
			hashKey ^= zobrist[sideToMove][BISHOPS][toSq];

			break;
		}

		if (c_piece == ROOKS) {

			if (toSq == 0) {

				hashKey ^= KEY_FLAG_WHITE_CASTLE_QUEEN_SIDE;
			} else if (toSq == 56) {

				hashKey ^= KEY_FLAG_BLACK_CASTLE_QUEEN_SIDE;
			} else if (toSq == 7) {

				hashKey ^= KEY_FLAG_WHITE_CASTLE_KING_SIDE;
			} else if (toSq == 63) {

				hashKey ^= KEY_FLAG_BLACK_CASTLE_KING_SIDE;
			}
		}

		break;
	}
}
