//
//  search.c
//  Vgen
//
//  Created by Amar Thapa on 23/12/18.
//  Copyright © 2018 Amar Thapa. All rights reserved.
//

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "search.h"
#include "evaluate.h"
#include "movegen.h"
#include "make_unmake.h"
#include "utility.h"
#include "vtime.h"
#include "nonslidingmoves.h"
#include "magicmoves.h"
#include "hash.h"

#define WINDOW 50
#define R 2

#define SCORE_PV_MOVE 30000
#define SCORE_HASH_MOVE 25000
#define SCORE_PROMOTION_MOVE 20005
#define SCORE_GOOD_CAPTURE_MOVE 20000
#define SCORE_KILLER_MOVE 15000
#define SCORE_ENPASSANT_MOVE 12000
#define SCORE_CASTLE_MOVE 10000
#define SCORE_DOUBLE_PUSH_MOVE 7000
#define SCORE_NORMAL_MOVE 5000
#define SCORE_BAD_CAPTURE_MOVE 3000

#define FALSE 0
#define TRUE 1

const int INFINITY = 30000;

int futility_margin[4] = { 150, 250, 400, 600 };
int seeVal[8] = { 0, 100, 300, 300, 500, 900, 2000, 0 };

#ifndef piece_name
char pieceName[2][8] = { { ' ', (char) 0, 'N', 'B', 'R', 'Q', 'K', '\0' }, {
		' ', (char) 0, 'n', 'b', 'r', 'q', 'k', '\0' } };
#endif

u32 pvMove;
char currentDepth;
void search(u8 sideToMove) {

	const int MATE = 25000;
	u32 move;
	u32 bestMove = 0;
	const u8 COLOR = sideToMove;
	nodes = 0;
	tbHits = 0;

	int alpha = -INFINITY;
	int beta = INFINITY;
	currentDepth = 1;
	int score;

	pvMove = 0UL;
	while (TRUE) {

		ply = 0;
		LINE mainline;

		score = alphabeta(COLOR, currentDepth, alpha, beta, MATE, &mainline, true);

		if (stopped || currentDepth > 50) {
			break;
		}

		if (score >= beta || score <= alpha) {

			alpha = -INFINITY;
			beta = INFINITY;
			continue;
		}

		alpha = score - WINDOW;
		beta = score + WINDOW;

		currentDepth++;

		bestMove = mainline.argmove[0];
		pvMove = bestMove;

		printf("info score cp %d depth %d nodes %llu time %llu ", score,
				currentDepth - 1, nodes, (getTimeMs() - startTime));
		printf("pv");

		int numberOfMoves = mainline.cmove;

		char str[10];
		for (int j = 0; j < numberOfMoves; j++) {
			str[0] = '\0';

			move = mainline.argmove[j];

			strcat(str, algebricPos(from_sq(move)));
			strcat(str, algebricPos(to_sq(move)));

			if (move_type(move) == MOVE_PROMOTION) {
				switch (promType(move)) {
				case PROMOTE_TO_ROOK:
					color ? strcat(str, "r") : strcat(str, "R");
					break;
				case PROMOTE_TO_BISHOP:
					color ? strcat(str, "b") : strcat(str, "B");
					break;
				case PROMOTE_TO_KNIGHT:
					color ? strcat(str, "n") : strcat(str, "N");
					break;
				default:
					color ? strcat(str, "q") : strcat(str, "Q");
					break;
				}
			}

			printf(" %s", str);
		}

		printf("\n");
	}

	char str[10];
	str[0] = '\0';

	strcat(str, algebricPos(from_sq(bestMove)));
	strcat(str, algebricPos(to_sq(bestMove)));

	if (move_type(bestMove) == MOVE_PROMOTION) {
		switch (promType(bestMove)) {
		case PROMOTE_TO_ROOK:
			color ? strcat(str, "r") : strcat(str, "R");
			break;
		case PROMOTE_TO_BISHOP:
			color ? strcat(str, "b") : strcat(str, "B");
			break;
		case PROMOTE_TO_KNIGHT:
			color ? strcat(str, "n") : strcat(str, "N");
			break;
		default:
			color ? strcat(str, "q") : strcat(str, "Q");
			break;
		}
	}

	printf("bestmove %s\n", str);

	timeSet = false;
	stopped = false;
}

void sortMoves(Move *moveList, u8 sideToMove, u8 numberOfMoves, u32 bestMove, u32 killerMove1, u32 killerMove2) {

	u8 moveType;
	for (u8 i = 0; i < numberOfMoves; i++) {

			moveType = move_type(moveList[i].move);
			if (moveList[i].move == pvMove) {

				moveList[i].score = SCORE_PV_MOVE;
			} else if (moveList[i].move == bestMove) {

				moveList[i].score = SCORE_HASH_MOVE;
			} else if (moveType == MOVE_CAPTURE) {
				if (moveList[i].score >= 0) {

					moveList[i].score = SCORE_GOOD_CAPTURE_MOVE + moveList[i].score * 0.5;
				} else {

					moveList[i].score = SCORE_BAD_CAPTURE_MOVE + moveList[i].score * 0.5;
				}
			} else if (moveType == MOVE_PROMOTION) {

				moveList[i].score += SCORE_PROMOTION_MOVE;
			} else if (moveList[i].move == killerMove1 || moveList[i].move == killerMove2) {

				moveList[i].score = SCORE_KILLER_MOVE;
			} else if(moveType == MOVE_ENPASSANT) {
				
				moveList[i].score = SCORE_ENPASSANT_MOVE;
			} else if(moveType == MOVE_CASTLE) {
				
				moveList[i].score = SCORE_CASTLE_MOVE;
			} else if(moveType == MOVE_DOUBLE_PUSH) {
				
				moveList[i].score = SCORE_DOUBLE_PUSH_MOVE;
			} else if(moveType == MOVE_NORMAL) {
				
				moveList[i].score = SCORE_NORMAL_MOVE + 
					historyScore[sideToMove][pieceType(moveList[i].move)][to_sq(moveList[i].move)] * 0.10; 
			}
		}

		Move move;
		for (u8 i = 0; i < numberOfMoves; i++) {
			for (u8 j = i + 1; j < numberOfMoves; j++) {

				if (moveList[i].score < moveList[j].score) {

					move = moveList[i];
					moveList[i] = moveList[j];
					moveList[j] = move;
				}
			}
		}
}

int alphabeta(u8 sideToMove, char depth, int alpha, int beta, int mate, LINE *pline, bool nullMove) {

	assert(beta > alpha);

	Move bestMove;
	bestMove.move = 0UL;
	bestMove.score = 0;

	if (depth <= 0) {

		pline->cmove = 0;

		int score = Quiescense(sideToMove, alpha, beta);

		RecordHash(sideToMove, depth, score, hashfEXACT, bestMove, hashKey);

		return score;
	}

	if ((nodes & 2047) == 0) {
		checkUp();
	}

	// if not root node, check for repetition
	if(depth != currentDepth) {
		if(checkForRepetition(hashKey, sideToMove)) {

			return 0;
		}
	}

	insertRepetitionHashKey(hashKey, sideToMove);
	
	u8 hashf = hashfALPHA;
	int value = ProbeHash(sideToMove, depth, alpha, beta, &bestMove, hashKey);

	if (value != VAL_UNKNOWN) {

		removeRepetitionHashKey();
		return value;
	}

	LINE line;
	if (nullMove) {
		if (!isKingInCheck(sideToMove) && popCount(occupied) > 16) {

			LINE nullLine;

			ply++;
			moveStack[ply].epFlag = 0;
			moveStack[ply].epSquare = 0;
			moveStack[ply].castleFlags = moveStack[ply - 1].castleFlags;

			int nullScore = -alphabeta(sideToMove ^ 1, depth - 1 - R, -beta, -beta + 1, mate - 1, &nullLine, false);

			if (nullScore >= beta) {
				ply--;

				RecordHash(sideToMove, depth, beta, hashfBETA, bestMove, hashKey);

				removeRepetitionHashKey();
				return beta;
			}

			ply--;
		}
	}

	nodes++;

	ply++;
	moveStack[ply].epFlag = moveStack[ply - 1].epFlag;
	moveStack[ply].epSquare = moveStack[ply - 1].epSquare;
	moveStack[ply].castleFlags = moveStack[ply - 1].castleFlags;

	int numberOfMoves;
	Move moveList[MAX_MOVES];
	numberOfMoves = genMoves(moveList, sideToMove);

	u32 killerMove1 = killerMoves[ply].killerMove1;
	u32 killerMove2 = killerMoves[ply].killerMove2;

	// sort moves generated
	sortMoves(&moveList, sideToMove, numberOfMoves, bestMove.move, killerMove1, killerMove2);

	int val = 0;
	int legalMoves = 0;
	int movesSearched = 0;
	bool isLmr = false;
	Move move;
	for (int i = 0; i < numberOfMoves; i++) {

		move = moveList[i];
		make_move(move.move);

		if (!isKingInCheck(sideToMove)) {

			legalMoves++;

			if(move_type(move.move) == MOVE_NORMAL) {

				// futility pruning
				if (depth > 5 && movesSearched > 0) {
					if (!isKingInCheck(sideToMove ^ 1)) {
						if ((evaluate(sideToMove) + futility_margin[depth - 1]) < alpha) {

							unmake_move(move.move);
							continue;
						}
					}
				}

				// Late Move Reduction
				if (depth > 3 && movesSearched > 4) {
					if (!isKingInCheck(sideToMove ^ 1) && pieceType(move.move) != PAWNS) {

						isLmr = true;
						int reduce = (depth < 12) ? 1 : 2;

						val = -alphabeta(sideToMove ^ 1, depth - 1 - reduce, -alpha - 1, -alpha, mate - 1, &line, true);
						if (val > alpha) {

							// Normal Alpha Beta Search
							val = -alphabeta(sideToMove ^ 1, depth - 1, -beta, -alpha, mate - 1, &line, true);
						}
					}
				}
			}
			
			if(!isLmr) {
				
				val = -alphabeta(sideToMove ^ 1, depth - 1, -beta, -alpha, mate - 1, &line, true);	
			}
			
			isLmr = false;

			movesSearched++;
			unmake_move(move.move);

			if (stopped) {

				return 0;
			}

			if (val >= beta) {

				RecordHash(sideToMove, depth, beta, hashfBETA, move, hashKey);
				if (move_type(move.move) == MOVE_NORMAL) {

					if (move.move != killerMove1) {

						killerMoves[ply].killerMove2 = killerMove1;
						killerMoves[ply].killerMove1 = move.move;
					}

					historyScore[sideToMove][pieceType(move.move)][to_sq(move.move)] += 1 << depth;
				}

				ply--;

				removeRepetitionHashKey();

				return beta;
			}

			if (val > alpha) {

				alpha = val;
				hashf = hashfEXACT;
				bestMove = move;

				pline->argmove[0] = move.move;
				memcpy(pline->argmove + 1, line.argmove, line.cmove * sizeof(u32));
				pline->cmove = line.cmove + 1;
			}
		} else {
			unmake_move(move.move);

			if (stopped) {

				return 0;
			}
		}
	}

	if (legalMoves == 0) {

		bestMove.move = 0ULL;
		if (!isKingInCheck(sideToMove)) {

			// stalemate
			alpha = 0;
		} else {

			// checkmate
			alpha = -mate;
		}
	}

	RecordHash(sideToMove, depth, alpha, hashf, bestMove, hashKey);

	ply--;

	removeRepetitionHashKey();

	return alpha;
}

int Quiescense(u8 sideToMove, int alpha, int beta) {

	assert(beta > alpha);
	if ((nodes & 2047) == 0) {
		checkUp();
	}

	if(checkForRepetition(hashKey, sideToMove)) {

		return 0;
	}

	insertRepetitionHashKey(hashKey, sideToMove);

	nodes++;

	int standingPat = evaluate(sideToMove);

	if (standingPat >= beta) {

		removeRepetitionHashKey();
		return beta;
	}

	if (standingPat > alpha) {
		alpha = standingPat;
	}

	u8 numberOfCaptures = 0;
	Move moveList[MAX_MOVES];

	numberOfCaptures = genAttacksQuies(moveList, sideToMove);

	for (int i = 0; i < numberOfCaptures; i++) {

		moveList[i].score = seeCapture(moveList[i].move, sideToMove);
	}

	Move move;
	for (int i = 0; i < numberOfCaptures; i++) {
		for (int j = i + 1; j < numberOfCaptures; j++) {

			if (moveList[i].score < moveList[j].score) {

				move = moveList[i];
				moveList[i] = moveList[j];
				moveList[j] = move;
			}
		}
	}

	for (int i = 0; i < numberOfCaptures; i++) {

		//if (moveList[i].score >= 0) {

			// delta pruning
//			if (cPieceType(moveList[i].move) != DUMMY &&
//					((standingPat + seeVal[cPieceType(moveList[i].move)] + 100) < alpha)) {
//
//				continue;
//			}

			make_move(moveList[i].move);
			if (!isKingInCheck(sideToMove)) {

				int score = -Quiescense(sideToMove ^ 1, -beta, -alpha);
				unmake_move(moveList[i].move);

				if (score >= beta) {

					removeRepetitionHashKey();
					return beta;
				}

				if (score > alpha) {
					alpha = score;
				}
			} else {
				unmake_move(moveList[i].move);
			}
		//}
	}

	removeRepetitionHashKey();
	return alpha;
}

int seeCapture(u32 move, u8 sideToMove) {

	u8 from = from_sq(move);
	u8 to = to_sq(move);
	u8 piece = pieceType(move);
	u8 cPiece = cPieceType(move);

	return see(to, cPiece, from, piece, sideToMove);
}

void debugSEE(u32 move, u8 sideToMove) {

	int value = 0;

	u8 from = from_sq(move);
	u8 to = to_sq(move);
	u8 piece = pieceType(move);
	u8 cPiece = cPieceType(move);

	//value = cPiece - seeOld(piece, to, sideToMove ^ 1);
	value = see(to, cPiece, from, piece, sideToMove);
	printf("%d \n", value);
}

int see(u8 toSq, u8 cPiece, u8 frSq, u8 aPiece, u8 sideToMove) {

	int swapList[32];
	int index = 1;

	u64 fromSet = 1ULL << frSq;
	u64 occ = occupied;

	occ ^= fromSet;

	swapList[0] = 0;

	// Handle en-passant
	
	// See for promoting pawns
	/* if(aPiece == PAWNS && (sideToMove ? fromSet & RANK_2 : fromSet & RANK_7)) {
		
		if(cPiece != DUMMY) {

			swapList[0] = seeVal[cPiece];
		}

		swapList[0] += seeVal[QUEEN] - seeVal[PAWNS];
		cPiece = QUEEN;
	} else {

		swapList[0] = seeVal[cPiece];
		cPiece = aPiece;
	} */
	
	swapList[0] = seeVal[cPiece];
	cPiece = aPiece;
	
	assert(swapList[0] > 0);

	u8 side = sideToMove ^ 1;

	u64 sideToMoveAttackers = attacksTo(occ, toSq, side);

	if (!sideToMoveAttackers) {

		return swapList[0];
	}

	u64 attackers = attacksTo(occ, toSq, WHITE) | attacksTo(occ, toSq, BLACK);

	u64 rooks = pieceBB[WHITE][ROOKS] | pieceBB[BLACK][ROOKS];
	u64 bishops = pieceBB[WHITE][BISHOPS] | pieceBB[BLACK][BISHOPS];
	u64 queens = pieceBB[WHITE][QUEEN] | pieceBB[BLACK][QUEEN];

	do {

		for (aPiece = PAWNS; !(fromSet = (sideToMoveAttackers & pieceBB[side][aPiece])); aPiece++) {

			assert(aPiece < KING);
		}

		occ ^= (fromSet & -fromSet);

		attackers |= ((rooks | queens) & Rmagic(toSq, occ)) | ((bishops | queens) & Bmagic(toSq, occ));
		attackers &= occ;

		swapList[index] = -swapList[index - 1] + seeVal[cPiece];

		cPiece = aPiece;

		index++;
		side ^= 1;

		sideToMoveAttackers = attackers & pieceBB[side][PIECES];

		// Stop after a king capture
		if (aPiece == KING && sideToMoveAttackers) {
			swapList[index++] = seeVal[KING];
			break;
		}
	} while (sideToMoveAttackers);

	while (--index) {
	
		if(swapList[index - 1] > -swapList[index]) {
			
			swapList[index - 1] = -swapList[index]; 
		}		
	}

	return swapList[0];
}

u64 considerXrays(u64 occ, u8 square) {

	u64 attacks = 0ULL;
	u64 sqBitboard = 1ULL << square;

	if (occ) {
		attacks |= (((sqBitboard << 7) & NOT_H_FILE)
				| ((sqBitboard << 9) & NOT_A_FILE)) & pieceBB[BLACK][PAWNS];

		attacks |= (((sqBitboard >> 7) & NOT_A_FILE)
				| ((sqBitboard >> 9) & NOT_H_FILE)) & pieceBB[WHITE][PAWNS];

		/* check if a bishop is attacking a square */
		attacks |= Bmagic(square, occ)
						& (pieceBB[WHITE][BISHOPS] | pieceBB[BLACK][BISHOPS]
																	| pieceBB[WHITE][QUEEN] | pieceBB[BLACK][QUEEN]);

		/* check if a rook is attacking a square */
		attacks |= Rmagic(square, occ)
						& (pieceBB[WHITE][ROOKS] | pieceBB[BLACK][ROOKS]
																  | pieceBB[WHITE][QUEEN] | pieceBB[BLACK][QUEEN]);
	}

	return attacks;
}

u64 attacksTo(u64 occ, u8 square, u8 sideToMove) {

	u64 attacks = 0ULL;
	u64 sqBitboard = 1ULL << square;

	if (sideToMove) {
		attacks |= (((sqBitboard << 7) & NOT_H_FILE)
				| ((sqBitboard << 9) & NOT_A_FILE)) & pieceBB[BLACK][PAWNS];
	} else {
		attacks |= (((sqBitboard >> 7) & NOT_A_FILE)
				| ((sqBitboard >> 9) & NOT_H_FILE)) & pieceBB[WHITE][PAWNS];
	}

	/* check if a knight is attacking a square */
	attacks |= get_knight_attacks(square) & pieceBB[sideToMove][KNIGHTS];

	/* check if a bishop or queen is attacking a square */
	attacks |= Bmagic(square, occ)
					& (pieceBB[sideToMove][BISHOPS] | pieceBB[sideToMove][QUEEN]);

	/* check if a rook or queen is attacking a square */
	attacks |= Rmagic(square, occ)
					& (pieceBB[sideToMove][ROOKS] | pieceBB[sideToMove][QUEEN]);

	/* check if a king is attacking a sq */
	attacks |= get_king_attacks(square) & pieceBB[sideToMove][KING];

	return attacks;
}

u64 getLeastValuablePiece(u64 attadef, u8 sideToMove, u8 *piece) {

	for (*piece = PAWNS; *piece <= KING; *piece += 1) {

		u64 subset = attadef & pieceBB[sideToMove][*piece];
		if (subset) {
			return subset & -subset;
		}
	}

	return 0; // empty set
}

u8 get_smallest_attacker(u8 square, u8 sideToMove, u8 *from) {

	u64 attacks;

	/* check if a pawn is attacking a square */

	u64 squareBitboard = 0ULL;
	if (sideToMove) {

		attacks = ((index_bb[square] << 7) & NOT_H_FILE)
						| ((index_bb[square] << 9) & NOT_A_FILE);

		squareBitboard = attacks & pieceBB[sideToMove][PAWNS];

		*from = bitScanForward(squareBitboard);

		if (squareBitboard) {

			return PAWNS;
		}
	} else {

		attacks = ((index_bb[square] >> 7) & NOT_A_FILE)
						| ((index_bb[square] >> 9) & NOT_H_FILE);

		squareBitboard = attacks & pieceBB[sideToMove][PAWNS];

		*from = bitScanForward(squareBitboard);
		if (squareBitboard) {

			return PAWNS;
		}
	}

	/* check if a knight is attacking a square */

	attacks = get_knight_attacks(square);
	squareBitboard = attacks & pieceBB[sideToMove][KNIGHTS];

	if (squareBitboard) {

		*from = bitScanForward(squareBitboard);

		return KNIGHTS;
	}

	/* check if a bishop is attacking a square */

	attacks = Bmagic(square, occupied);
	squareBitboard = attacks & pieceBB[sideToMove][BISHOPS];
	if (squareBitboard) {

		*from = bitScanForward(squareBitboard);
		return BISHOPS;
	}

	/* check if a rook is attacking a square */

	attacks = Rmagic(square, occupied);
	squareBitboard = attacks & pieceBB[sideToMove][ROOKS];
	if (squareBitboard) {

		*from = bitScanForward(squareBitboard);
		return ROOKS;
	}

	/* check if a queen is attacking a sq */

	attacks = Qmagic(square, occupied);
	squareBitboard = attacks & pieceBB[sideToMove][QUEEN];
	if (squareBitboard) {

		*from = bitScanForward(squareBitboard);
		return QUEEN;
	}

	/* check if a king is attacking a sq */

	attacks = get_king_attacks(square);
	squareBitboard = attacks & pieceBB[sideToMove][KING];
	if (squareBitboard) {

		*from = bitScanForward(squareBitboard);
		return KING;
	}

	// no piece attacking the square
	return 0;
}

void MVV_LVA(Move *moveList, u8 numberOfMoves) {

	u8 cPiece;
	u8 piece;
	for (int k = 0; k < numberOfMoves; k++) {

		cPiece = cPieceType(moveList[k].move);
		piece = pieceType(moveList[k].move);

		moveList[k].score = (64 * cPiece) - piece;
	}

	Move tempMove;
	for (int i = 0; i < numberOfMoves; i++) {
		for (int j = i + 1; j < numberOfMoves; j++) {

			if (moveList[i].score < moveList[j].score) {
				/* For decreasing order use < */
				tempMove = moveList[i];
				moveList[i] = moveList[j];
				moveList[j] = tempMove;
			}
		}
	}
}

/*for(int i = 0; i < numberOfMoves; i++) {
 for(int j = i + 1; j < numberOfMoves; j++) {

 if(moveList[i].score < moveList[j].score) {

 move = moveList[i];
 moveList[i] = moveList[j];
 moveList[j] = move;
 }
 }
 }*/

