//
//  evaluate.c
//  Vgen
//
//  Created by Amar Thapa on 23/12/18.
//  Copyright © 2018 Amar Thapa. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>

#include "evaluate.h"
#include "utility.h"
#include "movegen.h"
#include "make_unmake.h"
#include "psqt.h"

int Mirror64[64] = {
	56,	57,	58,	59,	60,	61,	62,	63,
	48,	49,	50,	51,	52,	53,	54,	55,
	40,	41,	42,	43,	44,	45,	46,	47,
	32,	33,	34,	35,	36,	37,	38,	39,
	24,	25,	26,	27,	28,	29,	30,	31,
	16,	17,	18,	19,	20,	21,	22,	23,
	8,	9,	10,	11,	12,	13,	14,	15,
	0,	1,	2,	3,	4,	5,	6,	7
};

int evaluate(u8 sideToMove) {

	int whiteScore = 0;
	int blackScore = 0;

	whiteScore = popCount(pieceBB[WHITE][KING]) * 2000 +
			popCount(pieceBB[WHITE][QUEEN]) * 900 +
			popCount(pieceBB[WHITE][ROOKS]) * 500 +
			popCount(pieceBB[WHITE][KNIGHTS]) * 300 +
			popCount(pieceBB[WHITE][BISHOPS]) * 300 +
			popCount(pieceBB[WHITE][PAWNS]) * 100;

	blackScore = popCount(pieceBB[BLACK][KING]) * 2000 +
			popCount(pieceBB[BLACK][QUEEN]) * 900 +
			popCount(pieceBB[BLACK][ROOKS]) * 500 +
			popCount(pieceBB[BLACK][KNIGHTS]) * 300 +
			popCount(pieceBB[BLACK][BISHOPS]) * 300 +
			popCount(pieceBB[BLACK][PAWNS]) * 100;

	int materialAdvantage  = sideToMove ? (blackScore - whiteScore) : (whiteScore - blackScore);
	int pieceSquareScore = sideToMove ? pSqTScore(BLACK) - pSqTScore(WHITE) : pSqTScore(WHITE) - pSqTScore(BLACK);

	//int doublePawnsScore = -30 * (sideToMove ? numOfDoublePawns(BLACK) - numOfDoublePawns(WHITE) : numOfDoublePawns(WHITE) - numOfDoublePawns(BLACK));
	//int backWardPawnScore = -70 * (sideToMove ? countBackWardPawns(BLACK) - countBackWardPawns(WHITE) : countBackWardPawns(WHITE) - countBackWardPawns(BLACK));

	return materialAdvantage + pieceSquareScore; //+ backWardPawnScore + doublePawnsScore;
}

int pSqTScore(u8 sideToMove) {
	int score = 0;

	int count = popCount(occupied);

	if(count <= 16) {
		if(sideToMove) {

			score += pstKingEg[Mirror64[bitScanForward(pieceBB[BLACK][KING])]];

			u64 bitboard1 = pieceBB[BLACK][PAWNS];

			while(bitboard1) {
				score += pstPawnEg[Mirror64[bitScanForward(bitboard1)]];	
				bitboard1 &= bitboard1 - 1; 
			}

			u64 bitboard2 = pieceBB[BLACK][ROOKS];
			while(bitboard2) {
				score += pstRookEg[Mirror64[bitScanForward(bitboard2)]];
				bitboard2 &= bitboard2 - 1;
			}

			u64 bitboard3 = pieceBB[BLACK][KNIGHTS];
			while(bitboard3) {
				score += pstKnightEg[Mirror64[bitScanForward(bitboard3)]];
				bitboard3 &= bitboard3 - 1;
			}

			u64 bitboard4 = pieceBB[BLACK][BISHOPS];
			while(bitboard4) {
				score += pstBishopEg[Mirror64[bitScanForward(bitboard4)]];
				bitboard4 &= bitboard4 - 1;
			}

			u64 bitboard5 = pieceBB[BLACK][QUEEN];
			while(bitboard5) {
				score += pstQueenEg[Mirror64[bitScanForward(bitboard5)]];
				bitboard5 &= bitboard5 - 1;
			}
		} else {

			score += pstKingEg[bitScanForward(pieceBB[WHITE][KING])];

			u64 bitboard1 = pieceBB[WHITE][PAWNS];

			while(bitboard1) {
				score += pstPawnEg[bitScanForward(bitboard1)];	
				bitboard1 &= bitboard1 - 1; 
			}

			u64 bitboard2 = pieceBB[WHITE][ROOKS];
			while(bitboard2) {
				score += pstRookEg[bitScanForward(bitboard2)];
				bitboard2 &= bitboard2 - 1;
			}

			u64 bitboard3 = pieceBB[WHITE][KNIGHTS];
			while(bitboard3) {
				score += pstKnightEg[bitScanForward(bitboard3)];
				bitboard3 &= bitboard3 - 1;
			}

			u64 bitboard4 = pieceBB[WHITE][BISHOPS];
			while(bitboard4) {
				score += pstBishopEg[bitScanForward(bitboard4)];
				bitboard4 &= bitboard4 - 1;
			}

			u64 bitboard5 = pieceBB[WHITE][QUEEN];
			while(bitboard5) {
				score += pstQueenEg[bitScanForward(bitboard5)];
				bitboard5 &= bitboard5 - 1;
			}
		}	
	} else {
		if(sideToMove) {

			score += pstKingMg[Mirror64[bitScanForward(pieceBB[BLACK][KING])]];

			u64 bitboard1 = pieceBB[BLACK][PAWNS];

			while(bitboard1) {
				score += pstPawnMg[Mirror64[bitScanForward(bitboard1)]];	
				bitboard1 &= bitboard1 - 1; 
			}

			u64 bitboard2 = pieceBB[BLACK][ROOKS];
			while(bitboard2) {
				score += pstRookMg[Mirror64[bitScanForward(bitboard2)]];
				bitboard2 &= bitboard2 - 1;
			}

			u64 bitboard3 = pieceBB[BLACK][KNIGHTS];
			while(bitboard3) {
				score += pstKnightMg[Mirror64[bitScanForward(bitboard3)]];
				bitboard3 &= bitboard3 - 1;
			}

			u64 bitboard4 = pieceBB[BLACK][BISHOPS];
			while(bitboard4) {
				score += pstBishopMg[Mirror64[bitScanForward(bitboard4)]];
				bitboard4 &= bitboard4 - 1;
			}

			u64 bitboard5 = pieceBB[BLACK][QUEEN];
			while(bitboard5) {
				score += pstQueenMg[Mirror64[bitScanForward(bitboard5)]];
				bitboard5 &= bitboard5 - 1;
			}
		} else {

			score += pstKingMg[bitScanForward(pieceBB[WHITE][KING])];

			u64 bitboard1 = pieceBB[WHITE][PAWNS];

			while(bitboard1) {
				score += pstPawnMg[bitScanForward(bitboard1)];	
				bitboard1 &= bitboard1 - 1; 
			}

			u64 bitboard2 = pieceBB[WHITE][ROOKS];
			while(bitboard2) {
				score += pstRookMg[bitScanForward(bitboard2)];
				bitboard2 &= bitboard2 - 1;
			}

			u64 bitboard3 = pieceBB[WHITE][KNIGHTS];
			while(bitboard3) {
				score += pstKnightMg[bitScanForward(bitboard3)];
				bitboard3 &= bitboard3 - 1;
			}

			u64 bitboard4 = pieceBB[WHITE][BISHOPS];
			while(bitboard4) {
				score += pstBishopMg[bitScanForward(bitboard4)];
				bitboard4 &= bitboard4 - 1;
			}

			u64 bitboard5 = pieceBB[WHITE][QUEEN];
			while(bitboard5) {
				score += pstQueenMg[bitScanForward(bitboard5)];
				bitboard5 &= bitboard5 - 1;
			}
		}
	}

	return score;
}

int numOfDoublePawns(u8 side) {

	u8 sq;
	int count = 0;
	u64 pawnsOnFile;
	u64 pawns = pieceBB[side][PAWNS];

	while(pawns) {

		sq = bitScanForward(pawns);
		pawnsOnFile = arrayTableDoublePawns[sq & 7] & pieceBB[side][PAWNS];
		if(pawnsOnFile & (pawnsOnFile - 1)) {

			count++;
		}

		pawns &= pawns - 1;
	}

	return count / 2;
}

void initTableDoublePawns() {

	arrayTableDoublePawns[0] = A_FILE;
	arrayTableDoublePawns[1] = B_FILE;
	arrayTableDoublePawns[2] = C_FILE;
	arrayTableDoublePawns[3] = D_FILE;
	arrayTableDoublePawns[4] = E_FILE;
	arrayTableDoublePawns[5] = F_FILE;
	arrayTableDoublePawns[6] = G_FILE;
	arrayTableDoublePawns[7] = H_FILE;
}

int countBackWardPawns(u8 side) {

	int count = 0;

	if(side) {

		count = popCount(bBackward(pieceBB[BLACK][PAWNS], pieceBB[WHITE][PAWNS]));
	} else {

		count = popCount(wBackward(pieceBB[WHITE][PAWNS], pieceBB[BLACK][PAWNS]));
	}

	return count;
}


u64 wBackward(u64 wpawns, u64 bpawns) {
   u64 stops = wpawns << 8;

   u64 wAttackSpans = wEastAttackFrontSpans(wpawns)
                    | wWestAttackFrontSpans(wpawns);

   u64 bAttacks     = bPawnEastAttacks(bpawns)
                    | bPawnWestAttacks(bpawns);

   return (stops & bAttacks & ~wAttackSpans) >> 8;
}

u64 bBackward(u64 bpawns, u64 wpawns) {
   u64 stops = bpawns >> 8;
   u64 bAttackSpans = bEastAttackFrontSpans(bpawns)
                    | bWestAttackFrontSpans(bpawns);

   u64 wAttacks     = wPawnEastAttacks(wpawns)
                    | wPawnWestAttacks(wpawns);

   return (stops & wAttacks & ~bAttackSpans) << 8;
}




























