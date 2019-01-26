//
//  evaluate.c
//  Vgen
//
//  Created by Amar Thapa on 23/12/18.
//  Copyright © 2018 Amar Thapa. All rights reserved.
//

#include "evaluate.h"
#include "utility.h"
#include "movegen.h"
#include "make_unmake.h"

int wKM_PSqT[64] = { 
	-20, 0, 0, -10, -10, 0, 0, -20, 	 	 
	-30, -30, -30, -35, -35, -30, -30, -30, 	 	 
	-40, -40, -45, -50, -50, -45, -40, -40, 	 	 
	-50, -50, -55, -60, -60, -55, -50, -50, 	 	 
	-55, -55, -60, -70, -70, -60, -55, -55, 	 	 
	-55, -55, -60, -70, -70, -60, -55, -55, 	 	 
	-55, -55, -60, -70, -70, -60, -55, -55, 	 	 
	-55, -55, -60, -70, -70, -60, -55, -55};

int wKE_PSqT[64] = {
	-30, -25, -15, -10, -10, -15, -25, -30, 	 	 
	-15, -10, 0, 10, 10, 0, -10, -15, 	 	 
	0, 15, 30, 40, 40, 30, 15, 0, 	 	 
	10, 25, 40, 50, 50, 40, 25, 10, 	 	 
	10, 25, 40, 50, 50, 40, 25, 10, 	 	 
	10, 25, 40, 50, 50, 40, 25, 10, 	 	 
	0, 20, 35, 45, 45, 35, 20, 0, 	 	 
	-10, 10, 15, 20, 20, 15, 10, -10}; 	 	 

int bKM_PSqT[64] = {  	 	  	 	  	 	
	-55, -55, -60, -70, -70, -60, -55, -55, 	 	 
	-55, -55, -60, -70, -70, -60, -55, -55, 	 	 
	-55, -55, -60, -70, -70, -60, -55, -55, 	 	 
	-55, -55, -60, -70, -70, -60, -55, -55,
	-50, -50, -55, -60, -60, -55, -50, -50, 
	-40, -40, -45, -50, -50, -45, -40, -40,
	-30, -30, -30, -35, -35, -30, -30, -30,
	-20, 0, 0, -10, -10, 0, 0, -20};

int bKE_PSqT[64] = {	 	 
	-10, 10, 15, 20, 20, 15, 10, -10,	 	 	 
	0, 20, 35, 45, 45, 35, 20, 0, 	 	 
	10, 25, 40, 50, 50, 40, 25, 10, 	 	 
	10, 25, 40, 50, 50, 40, 25, 10, 	 	 
	10, 25, 40, 50, 50, 40, 25, 10,
	0, 15, 30, 40, 40, 30, 15, 0, 	
	-15, -10, 0, 10, 10, 0, -10, -15,
	-30, -25, -15, -10, -10, -15, -25, -30}; 	 	 

int wPM_PSqT[64] = {
	0, 0, 0, 0, 0, 0, 0, 0, 	 	 
	-21, -16, -6, -1, -1, -6, -16, -21, 	 	 
	-21, -16, -6, 4, 4, -6, -16, -21, 	 	 
	-21, -16, -1, 9, 9, -1, -16, -21, 	 	 
	-14, -8, 6, 17, 17, 6, -8, -14, 	 	 
	-5, 1, 14, 29, 29, 14, 1, -5, 	 	 
	7, 11, 23, 39, 39, 23, 11, 7, 	 	 
	0, 0, 0, 0, 0, 0, 0, 0};

int wPE_PSqT[64] = {
	0, 0, 0, 0, 0, 0, 0, 0, 	 	 
	5, -10, -20, -25, -25, -20, -10, 5, 	 	 
	5, -10, -20, -25, -25, -20, -10, 5, 	 	 
	10, -5, -15, -20, -20, -15, -5, 10, 	 	 
	18, 2, -8, -15, -15, -8, 2, 18, 	 	 
	30, 14, 1, -10, -10, 1, 14, 30, 	 	 
	45, 30, 16, 5, 5, 16, 30, 45, 	 	 
	0, 0, 0, 0, 0, 0, 0, 0}; 	 	 	

int bPM_PSqT[64] = {
 	0, 0, 0, 0, 0, 0, 0, 0, 	  	  	  	  
	7, 11, 23, 39, 39, 23, 11, 7, 	 	
	-5, 1, 14, 29, 29, 14, 1, -5, 	 	
	-14, -8, 6, 17, 17, 6, -8, -14, 	
	-21, -16, -1, 9, 9, -1, -16, -21, 	
	-21, -16, -6, 4, 4, -6, -16, -21, 	
	-21, -16, -6, -1, -1, -6, -16, -21,
	0, 0, 0, 0, 0, 0, 0, 0};

int bPE_PSqT[64] = { 	 	 
	0, 0, 0, 0, 0, 0, 0, 0,	 	 
	45, 30, 16, 5, 5, 16, 30, 45, 		 	 
	30, 14, 1, -10, -10, 1, 14, 30,   	 	 	 
	18, 2, -8, -15, -15, -8, 2, 18, 
	10, -5, -15, -20, -20, -15, -5, 10,
	5, -10, -20, -25, -25, -20, -10, 5,
	5, -10, -20, -25, -25, -20, -10, 5,	
	0, 0, 0, 0, 0, 0, 0, 0};
	
int wRM_PSqT[64] = {
	-8, -6, 2, 7, 7, 2, -6, -8, 	 
	-8, -6, 2, 7, 7, 2, -6, -8, 	 
	-8, -6, 6, 7, 7, 6, -6, -8, 	 
	-8, -6, 6, 7, 7, 6, -6, -8, 	 
	-8, -6, 6, 8, 8, 6, -6, -8, 	 
	-8, -6, 6, 10, 10, 6, -6, -8, 	 
	2, 2, 7, 12, 12, 7, 2, 2, 	 	
	-8, -6, 2, 7, 7, 2, -6, -8};

int bRM_PSqT[64] = {	  	 	 	
	-8, -6, 2, 7, 7, 2, -6, -8,
	2, 2, 7, 12, 12, 7, 2, 2, 	 
	-8, -6, 6, 10, 10, 6, -6, -8,
	-8, -6, 6, 8, 8, 6, -6, -8, 
	-8, -6, 6, 7, 7, 6, -6, -8, 	 
	-8, -6, 6, 7, 7, 6, -6, -8,
	-8, -6, 2, 7, 7, 2, -6, -8, 	 
	-8, -6, 2, 7, 7, 2, -6, -8 };	
	
int wRE_PSqT[64] = {
	0, 0, 0, 0, 0, 0, 0, 0, 	 
	0, 0, 0, 0, 0, 0, 0, 0, 	 
	0, 0, 0, 0, 0, 0, 0, 0, 	 
	0, 0, 0, 0, 0, 0, 0, 0, 	 
	0, 0, 0, 0, 0, 0, 0, 0, 	 
	0, 0, 0, 0, 0, 0, 0, 0, 	 
	0, 0, 0, 0, 0, 0, 0, 0, 	 
	0, 0, 0, 0, 0, 0, 0, 0 };

int bRE_PSqT[64] = {
	0, 0, 0, 0, 0, 0, 0, 0, 	 
	0, 0, 0, 0, 0, 0, 0, 0, 	 
	0, 0, 0, 0, 0, 0, 0, 0, 	 
	0, 0, 0, 0, 0, 0, 0, 0, 	 
	0, 0, 0, 0, 0, 0, 0, 0, 	 
	0, 0, 0, 0, 0, 0, 0, 0, 	 
	0, 0, 0, 0, 0, 0, 0, 0, 	 
	0, 0, 0, 0, 0, 0, 0, 0 };	
	
int wKnightMidgamePSqT[64] = {
	-69, -19, -24, -14, -14, -24, -19, -69, 	 	 
	-54, -39, -9, 11, 11, -9, -39, -54, 	 	 
	-39, 1, 31, 21, 21, 31, 1, -39, 	 	 
	-39, 11, 41, 36, 36, 41, 11, -39, 	 	 
	-39, 41, 51, 51, 51, 51, 41, -39, 	 	 
	-39, 46, 61, 71, 71, 61, 46, -39, 	 	 
	-39, 21, 41, 41, 41, 41, 21, -39, 	 	 
	-59, -39, -29, -29, -29, -29, -39, -59}; 	 	 

int bKnightMidgamePSqt[64] = {
	-59, -39, -29, -29, -29, -29, -39, -59
	-39, 21, 41, 41, 41, 41, 21, -39, 	 	 
	-39, 46, 61, 71, 71, 61, 46, -39, 	 	 
	-39, 41, 51, 51, 51, 51, 41, -39, 	 	 
	-39, 11, 41, 36, 36, 41, 11, -39, 	 	 
	-39, 1, 31, 21, 21, 31, 1, -39, 	 	 
	-54, -39, -9, 11, 11, -9, -39, -54, 	 	 
	-69, -19, -24, -14, -14, -24, -19, -69};
	
int	wKnightEndgamePSqt[64] = {
	-63, -53, -43, -43, -43, -43, -53, -63, 	 	 
	-53, -43, 18, 28, 28, 18, -43, -53, 	 	 
	-43, 18, 48, 38, 38, 48, 18, -43, 	 	 
	-43, 38, 58, 68, 68, 58, 38, -43, 	 	 
	-43, 38, 73, 78, 78, 73, 38, -43, 	 	 
	-43, 28, 78, 73, 73, 78, 28, -43, 	 	 
	-53, -43, 38, 48, 48, 38, -43, -53, 	 	 
	-63, -53, -43, -43, -43, -43, -53, -63}; 	 	 

int bKnightEndgamePSqt[64] = {
	-63, -53, -43, -43, -43, -43, -53, -63,
	-53, -43, 38, 48, 48, 38, -43, -53, 	
	-43, 28, 78, 73, 73, 78, 28, -43, 	 	
	-43, 38, 73, 78, 78, 73, 38, -43, 	 	 
	-43, 38, 58, 68, 68, 58, 38, -43, 	 	
	-43, 18, 48, 38, 38, 48, 18, -43, 	 	
	-53, -43, 18, 28, 28, 18, -43, -53, 	
	-63, -53, -43, -43, -43, -43, -53, -63};

int wBishopMidgamePSqt[64] = {
	-30, -25, -20, -20, -20, -20, -25, -30, 	 	 
	-28, 11, 6, 1, 1, 6, 11, -28, 	 	 
	-25, 6, 16, 11, 11, 16, 6, -25, 	 	 
	1, 1, 16, 21, 21, 16, 1, 1, 	 	 
	1, 21, 21, 26, 26, 21, 21, 1, 	 	 
	1, 11, 21, 26, 26, 21, 11, 1, 	 	 
	-10, 11, 1, 1, 1, 1, 11, -10, 	 	 
	-20, -18, -16, -14, -14, -16, -18, -20};	

int bBishopMidgamePSqt[64] = {
	-20, -18, -16, -14, -14, -16, -18, -20,
	-10, 11, 1, 1, 1, 1, 11, -10, 	 	 
	1, 11, 21, 26, 26, 21, 11, 1, 	 	 
	1, 21, 21, 26, 26, 21, 21, 1, 	 	 
	1, 1, 16, 21, 21, 16, 1, 1, 	 	 
	-25, 6, 16, 11, 11, 16, 6, -25, 	 
	-28, 11, 6, 1, 1, 6, 11, -28, 	 	 
	-30, -25, -20, -20, -20, -20, -25, -30};

int wBishopEndgamePSqt[64] = {
	-38, -18, -8, 2, 2, -8, -18, -38, 	 	 
	-18, -8, 2, 7, 7, 2, -8, -18, 	 	 
	-8, 2, 10, 12, 12, 10, 2, -8, 	 	 
	2, 12, 16, 20, 20, 16, 12, 2, 	 	 
	2, 12, 17, 22, 22, 17, 12, 2, 	 	 
	-8, 2, 20, 22, 22, 20, 2, -8, 	 	 
	-18, -8, 0, 12, 12, 0, -8, -18, 	 	 
	-38, -18, -8, 2, 2, -8, -18, -38}; 	 	 

int bBishopEndgamePSqt[64] = {
	-38, -18, -8, 2, 2, -8, -18, -38,
	-18, -8, 0, 12, 12, 0, -8, -18, 
	-8, 2, 20, 22, 22, 20, 2, -8, 	
	2, 12, 17, 22, 22, 17, 12, 2, 	
	2, 12, 16, 20, 20, 16, 12, 2, 	
	-8, 2, 10, 12, 12, 10, 2, -8, 	
	-18, -8, 2, 7, 7, 2, -8, -18, 
	-38, -18, -8, 2, 2, -8, -18, -38};

int wQueenMidgamePSqt[64] = {
	-26, -16, -6, 4, 4, -6, -16, -26, 	 	 
	-16, -11, -1, 4, 4, -1, -11, -16, 	 	 
	-6, -6, -1, 4, 4, -1, -6, -6, 	 	 
	4, 4, 4, 4, 4, 4, 4, 4, 	 	 
	4, 4, 4, 4, 4, 4, 4, 4, 	 	 
	4, 4, 4, 4, 4, 4, 4, 4, 	 	 
	4, 4, 4, 4, 4, 4, 4, 4, 	 	 
	4, 4, 4, 4, 4, 4, 4, 4};

int bQueenMidgamePSqt[64] = {
	4, 4, 4, 4, 4, 4, 4, 4, 	 	 
	4, 4, 4, 4, 4, 4, 4, 4, 	 	 
	4, 4, 4, 4, 4, 4, 4, 4, 	 	 
	4, 4, 4, 4, 4, 4, 4, 4, 	 	 
	4, 4, 4, 4, 4, 4, 4, 4,
	-6, -6, -1, 4, 4, -1, -6, -6, 	 	 
	-16, -11, -1, 4, 4, -1, -11, -16, 	 	 
	-26, -16, -6, 4, 4, -6, -16, -26};	

int wQueenEndgamePSqt[64] = {
	-46, -41, -31, -26, -26, -31, -41, -46, 	 	 
	-31, -26, -16, -6, -6, -16, -26, -31, 	 	 
	-16, -1, 14, 24, 24, 14, -1, -16, 	 	 
	-6, 9, 24, 34, 34, 24, 9, -6, 	 	 
	-6, 9, 24, 34, 34, 24, 9, -6, 	 	 
	-6, 9, 24, 34, 34, 24, 9, -6, 	 	 
	-16, 4, 19, 29, 29, 19, 4, -16, 	 	 
	-26, -6, -1, 4, 4, -1, -6, -26}; 	 	 

int bQueenEndgamePSqt[64] = {
	-26, -6, -1, 4, 4, -1, -6, -26,
	-16, 4, 19, 29, 29, 19, 4, -16,
	-6, 9, 24, 34, 34, 24, 9, -6, 	 	 
	-6, 9, 24, 34, 34, 24, 9, -6, 	 	 
	-6, 9, 24, 34, 34, 24, 9, -6, 	
	-16, -1, 14, 24, 24, 14, -1, -16,
	-31, -26, -16, -6, -6, -16, -26, -31,
	-46, -41, -31, -26, -26, -31, -41, -46};

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
	
    //int mobilityScore = sideToMove ? (getMobility(BLACK) - getMobility(WHITE)) : (getMobility(WHITE) - getMobility(BLACK));
	int pieceSquareScore = sideToMove ? pSqTScore(BLACK) - pSqTScore(WHITE) : pSqTScore(WHITE) - pSqTScore(BLACK);
	
    return materialAdvantage + pieceSquareScore;// + mobilityScore;
 }

int getMobility(u8 sideToMove) {
	
	u8 count = 0;
	Move moveList[MAX_MOVES];

	count = genMoves(moveList, sideToMove);
		
	return count / 3;
}

int pSqTScore(u8 sideToMove) {
	int score = 0;
	
	int count = popCount(occupied);
	
	if(count <= 16) {
		if(sideToMove) {
			
			score += bKE_PSqT[bitScanForward(pieceBB[sideToMove][KING])];
			
			u64 bitboard1 = pieceBB[BLACK][PAWNS];
			
			while(bitboard1) {
				score += bPE_PSqT[bitScanForward(bitboard1)];	
				bitboard1 &= bitboard1 - 1; 
			}
			
			u64 bitboard2 = pieceBB[BLACK][ROOKS];
			while(bitboard2) {
				score += bRE_PSqT[bitScanForward(bitboard2)];
				bitboard2 &= bitboard2 - 1;
			}
			
			u64 bitboard3 = pieceBB[BLACK][KNIGHTS];
			while(bitboard3) {
				score += bKnightEndgamePSqt[bitScanForward(bitboard3)];
				bitboard3 &= bitboard3 - 1;
			}
			
			u64 bitboard4 = pieceBB[BLACK][BISHOPS];
			while(bitboard4) {
				score += bBishopEndgamePSqt[bitScanForward(bitboard4)];
				bitboard4 &= bitboard4 - 1;
			}
			
			u64 bitboard5 = pieceBB[BLACK][QUEEN];
			while(bitboard5) {
				score += bQueenEndgamePSqt[bitScanForward(bitboard5)];
				bitboard5 &= bitboard5 - 1;
			}
		} else {
			
			score += wKE_PSqT[bitScanForward(pieceBB[WHITE][KING])];
			
			u64 bitboard1 = pieceBB[WHITE][PAWNS];
			
			while(bitboard1) {
				score += wPE_PSqT[bitScanForward(bitboard1)];	
				bitboard1 &= bitboard1 - 1; 
			}
			
			u64 bitboard2 = pieceBB[WHITE][ROOKS];
			while(bitboard2) {
				score += wRE_PSqT[bitScanForward(bitboard2)];
				bitboard2 &= bitboard2 - 1;
			}
			
			u64 bitboard3 = pieceBB[WHITE][KNIGHTS];
			while(bitboard3) {
				score += wKnightEndgamePSqt[bitScanForward(bitboard3)];
				bitboard3 &= bitboard3 - 1;
			}
			
			u64 bitboard4 = pieceBB[WHITE][BISHOPS];
			while(bitboard4) {
				score += wBishopEndgamePSqt[bitScanForward(bitboard4)];
				bitboard4 &= bitboard4 - 1;
			}
			
			u64 bitboard5 = pieceBB[WHITE][QUEEN];
			while(bitboard5) {
				score += wQueenEndgamePSqt[bitScanForward(bitboard5)];
				bitboard5 &= bitboard5 - 1;
			}
		}	
	} else {
		if(sideToMove) {
			
			score += bKM_PSqT[bitScanForward(pieceBB[BLACK][KING])];
			
			u64 bitboard1 = pieceBB[BLACK][PAWNS];
			
			while(bitboard1) {
				score += bPM_PSqT[bitScanForward(bitboard1)];	
				bitboard1 &= bitboard1 - 1; 
			}
			
			u64 bitboard2 = pieceBB[BLACK][ROOKS];
			while(bitboard2) {
				score += bRM_PSqT[bitScanForward(bitboard2)];
				bitboard2 &= bitboard2 - 1;
			}
			
			u64 bitboard3 = pieceBB[BLACK][KNIGHTS];
			while(bitboard3) {
				score += bKnightMidgamePSqt[bitScanForward(bitboard3)];
				bitboard3 &= bitboard3 - 1;
			}
			
			u64 bitboard4 = pieceBB[BLACK][BISHOPS];
			while(bitboard4) {
				score += bBishopMidgamePSqt[bitScanForward(bitboard4)];
				bitboard4 &= bitboard4 - 1;
			}
			
			u64 bitboard5 = pieceBB[BLACK][QUEEN];
			while(bitboard5) {
				score += bQueenMidgamePSqt[bitScanForward(bitboard5)];
				bitboard5 &= bitboard5 - 1;
			}
		} else {
			
			score += wKM_PSqT[bitScanForward(pieceBB[sideToMove][KING])];
			
			u64 bitboard1 = pieceBB[WHITE][PAWNS];
			
			while(bitboard1) {
				score += wPM_PSqT[bitScanForward(bitboard1)];	
				bitboard1 &= bitboard1 - 1; 
			}
			
			u64 bitboard2 = pieceBB[WHITE][ROOKS];
			while(bitboard2) {
				score += wRM_PSqT[bitScanForward(bitboard2)];
				bitboard2 &= bitboard2 - 1;
			}
			
			u64 bitboard3 = pieceBB[WHITE][KNIGHTS];
			while(bitboard3) {
				score += wKnightMidgamePSqT[bitScanForward(bitboard3)];
				bitboard3 &= bitboard3 - 1;
			}
			
			u64 bitboard4 = pieceBB[WHITE][BISHOPS];
			while(bitboard4) {
				score += wBishopMidgamePSqt[bitScanForward(bitboard4)];
				bitboard4 &= bitboard4 - 1;
			}
			
			u64 bitboard5 = pieceBB[WHITE][QUEEN];
			while(bitboard5) {
				score += wQueenMidgamePSqt[bitScanForward(bitboard5)];
				bitboard5 &= bitboard5 - 1;
			}
		}
	}
	
	return score;
}





