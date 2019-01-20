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
    
const int INFINITY = 30000;

#ifndef piece_name
char pieceName[2][8] = { { ' ', (char) 0, 'N', 'B', 'R', 'Q', 'K', '\0'}, { ' ', (char) 0, 'n', 'b', 'r', 'q', 'k', '\0'}};
#endif

u32 pvMove;
void search(u8 sideToMove) {
	
    const int MATE = 25000;
    u32 move;
    u32 bestMove = 0;
    const u8 COLOR = sideToMove;
    nodes = 0;
	tbHits = 0;
	
	int alpha = -INFINITY;
	int beta = INFINITY;
	int currentDepth = 1;
	int score;
	
	pvMove = 0UL;
	while(1) {
    
	    ply = 0;
        LINE mainline;
		
		score = alphabeta(COLOR, currentDepth, alpha, beta, MATE, &mainline, true);
	
        if(stopped) {
            break;
        }
		
		if(score >= beta || score <= alpha) {
			
			alpha = -INFINITY;
			beta = INFINITY;
			continue;
		}
		
		alpha = score - WINDOW;
		beta = score + WINDOW;
		
		currentDepth++;
	
        bestMove = mainline.argmove[0];
		pvMove = bestMove;
		
        printf("info score cp %d depth %d nodes %llu time %llu ", score, currentDepth - 1, nodes, (getTimeMs() - startTime));
        printf("pv");
        
        int numberOfMoves = mainline.cmove;
		
		char str[10];	
        for (int j = 0; j < numberOfMoves; j++) {
			str[0] = '\0';
			
            move = mainline.argmove[j];

            strcat(str, algebricPos(from_sq(move)));
            strcat(str, algebricPos(to_sq(move)));

            if(move_type(move) == MOVE_PROMOTION) {
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

    if(move_type(bestMove) == MOVE_PROMOTION) {
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

const int noScore = 777;
int futility_margin[4] = {150, 250, 400, 600};  
int seeVal[8] = { 0, 100, 300, 300, 500, 900, 2000, 0}; 

int alphabeta(u8 sideToMove, int depth, int alpha, int beta, int mate, LINE *pline, bool nullMove) {
    	
	Move bestMove;
	bestMove.move = 0UL;
	bestMove.score = 0;
	
    int hashf = hashfALPHA;
    int value = ProbeHash(sideToMove, depth, alpha, beta, &bestMove);
	
    if(value != VAL_UNKNOWN) {
		
		return value;
    }

    if (depth <= 0) {
        
        pline->cmove = 0;
		
        int score = Quiescense(sideToMove, alpha, beta);
       
	    RecordHash(sideToMove, depth, score, hashfEXACT, bestMove);
    
		return score;
    }
	
    if((nodes & 2047) == 0) {
        checkUp();
    }
	
	if(insertRepetitionHashKey(hashKey, sideToMove)) {
	
		return 0;
	}
	
	if(!isKingInCheck(sideToMove)) {
		 
		if(nullMove && popCount(occupied) > 7) {
			LINE nullLine;
			
			ply++;
			moveStack[ply].epFlag = 0;
			moveStack[ply].epSquare = 0;
			moveStack[ply].castleFlags  =  moveStack[ply - 1].castleFlags;
	
			int nullScore = -alphabeta(sideToMove ^ 1, depth - 1 - R, -beta, -beta + 1, mate - 1, &nullLine, false);
			
			if (nullScore >= beta) {
				ply--;
	
				repIndex--;	
				return beta;
			}
			
			ply--;	
		}
	}  
	
	bool foundPV = false;
	
	LINE line; 
    nodes++;
	
    ply++;
    moveStack[ply].epFlag = moveStack[ply - 1].epFlag;
    moveStack[ply].epSquare = moveStack[ply - 1].epSquare;
    moveStack[ply].castleFlags  =  moveStack[ply - 1].castleFlags;
	
    int numberOfMoves;
    Move moveList[MAX_MOVES]; 
	
    numberOfMoves = genMoves(moveList, sideToMove); 
    
	u32 killerMove1 = killerMoves[ply].killerMove1;
	u32 killerMove2 = killerMoves[ply].killerMove2;
	
	u32 checkMove;
	for(int i = 0; i < numberOfMoves; i++) {
		
		checkMove = moveList[i].move;
		
		if(checkMove == pvMove) {
		
			moveList[i].score = 25000;
		} else if(checkMove == bestMove.move) {

			moveList[i].score = 20000;
		} else if(move_type(checkMove) == MOVE_CAPTURE) {
			if(moveList[i].score > -1) {
			
				moveList[i].score += 15000;
			} else {
				
				moveList[i].score += 2000;
			}				
		} else if(move_type(checkMove) == MOVE_PROMOTION) {
			
			moveList[i].score = 10000;
		} else if(checkMove == killerMove1 || checkMove == killerMove2) {
			moveList[i].score = 5000;
		} else {
			
			moveList[i].score = 4000;
		}
	}

	Move move;
	/* for (int i = 0; i < (numberOfMoves - 1); i++) {
		int position = i;
   
		for (int j = i + 1; j < numberOfMoves; j++) {
			
			if (moveList[i].score < moveList[j].score) {
				position = j;
			}
		}
	
		if (position != i) {
		
			move = moveList[i];
			moveList[i] = moveList[position];
			moveList[position] = move;
		}
	}
	 */
	for(int i = 0; i < numberOfMoves; i++) {
		for(int j = i + 1; j < numberOfMoves; j++) {
			
			if(moveList[i].score < moveList[j].score) {
				
				move = moveList[i];
				moveList[i] = moveList[j];
				moveList[j] = move;
			}
		}
	}	
	
	int legalMoves = 0;
	int movesSearched = 0;
	for (int i = 0; i < numberOfMoves; i++) {
         
		move = moveList[i];
        make_move(move.move);
		
		if (!isKingInCheck(sideToMove)) {
			
			legalMoves++;
			
			int val;
			int moveType = move_type(move.move);
			
			if(moveType == MOVE_NORMAL ||  
					moveType == MOVE_DOUBLE_PUSH ||
						moveType == MOVE_CASTLE)  {
				
				// frutility pruning
				if(depth > 5 && movesSearched > 0) {	
					if((evaluate(sideToMove) + futility_margin[depth - 1]) < alpha) {
						unmake_move(move.move);
						continue;
					}
				}
				
				// Late Move Reduction
				if(movesSearched > 4 && depth > 3 && !foundPV && move.score < 5000) {
					
					val = -alphabeta(sideToMove ^ 1, depth - 2, -alpha - 1, -alpha, mate - 1, &line, true);	
				} else {
					
					val = alpha + 1;
				}
			} else {
				
				val = alpha + 1; 					
			}
		
			movesSearched++;
			
			if (foundPV) {
				// Principal Variation Search
				val = -alphabeta(sideToMove ^ 1, depth - 1, -alpha - 1, -alpha, mate - 1, &line, true);	
				if ((val > alpha) && (val < beta)) {
					
					// Check for failure.
					val = -alphabeta(sideToMove ^ 1, depth - 1, -beta, -alpha, mate - 1, &line, true);	
				}
			} else {
				if(val > alpha) {
					// Normal Alpha Beta Search
					val = -alphabeta(sideToMove ^ 1, depth - 1, -beta, -alpha, mate - 1, &line, true);	
				}
			}
			
			unmake_move(move.move);
            
            if(stopped) {
			    return 0;
            }
            
            if (val >= beta) {
                RecordHash(sideToMove, depth, beta, hashfBETA, move);
				repIndex--;
		
				if(moveType == MOVE_NORMAL ||  
						moveType == MOVE_DOUBLE_PUSH ||
							moveType == MOVE_CASTLE) {
					
					if(move.move != killerMoves[ply].killerMove1) {
						
						killerMoves[ply].killerMove2 = killerMoves[ply].killerMove1;
						killerMoves[ply].killerMove1 = move.move;
					}							
					
					historyScore[sideToMove][pieceType(move.move)][to_sq(move.move)] += 1 << depth;
				}
				
				ply--;
                
                return beta;
            }
            
            if (val > alpha) {				
				
				foundPV = true;
				
				alpha = val;
				
				hashf = hashfEXACT;
                bestMove = move;
				
 			    pline->argmove[0] = move.move;
                memcpy(pline->argmove + 1, line.argmove, line.cmove * sizeof(u32));
                pline->cmove = line.cmove + 1;
            }
        } else {
            unmake_move(move.move);
            
            if(stopped) {
				
                return 0;
            }
        }
    }
	
    if(legalMoves == 0) {
		bestMove.move = 0ULL;
        if (!isKingInCheck(sideToMove)) {
            
			// stalemate
			alpha = 0;
        } else {
			
            // checkmate
            alpha = -mate;
        }
    }
    
    RecordHash(sideToMove, depth, alpha, hashf, bestMove);
	repIndex--;
		
    ply--;
    return alpha;
}

int Quiescense(u8 sideToMove, int alpha, int beta) {

    if((nodes & 2047) == 0) {
        checkUp();
    }
	
    nodes++;

	if(insertRepetitionHashKey(hashKey, sideToMove)) {
		return 0;
	}
    
	int standingPat = evaluate(sideToMove);
    
    if (standingPat >= beta) {
		
     	repIndex--;	
		return beta;
    }

    if(standingPat > alpha) {
        alpha = standingPat;
    }
    
    u8 numberOfCaptures = 0;
    Move moveList[MAX_MOVES];
    
    numberOfCaptures = genAttacksQuies(moveList, sideToMove);
	
	for(int i = 0; i < numberOfCaptures; i++) {
		
		moveList[i].score = seeCapture(moveList[i].move, sideToMove);
	}

	Move move;
	/* for (int i = 0; i < (numberOfCaptures - 1); i++) {
		int position = i;
   
		for (int j = i + 1; j < numberOfCaptures; j++) {
			
			if (moveList[i].score < moveList[j].score) {
				position = j;
			}
		}
	
		if (position != i) {
		
			move = moveList[i];
			moveList[i] = moveList[position];
			moveList[position] = move;
		}
	} */
		
	for(int i = 0; i < numberOfCaptures; i++) {
		for(int j = i + 1; j < numberOfCaptures; j++) {
			
			if(moveList[i].score < moveList[j].score) {
				
				move = moveList[i];
				moveList[i] = moveList[j];
				moveList[j] = move;
			}
		}
	}	

	for (int i = 0; i < numberOfCaptures; i++) {
			
		if(moveList[i].score >= 0) {
			make_move(moveList[i].move);
		
			if (!isKingInCheck(sideToMove)) {
				
				int score = -Quiescense(sideToMove ^ 1, -beta, -alpha);
            
				unmake_move(moveList[i].move);
				
				if (score >= beta) {
					
					repIndex--;
					return beta;
				}
            
				if(score > alpha) {
					alpha = score;
				}
			} else {
					
				unmake_move(moveList[i].move);
			}
		}
	}
	
	repIndex--;
    return alpha;
}

int seeCapture(u32 move, u8 sideToMove) {

	int value = 0;
	
	u8 from = from_sq(move);
	u8 to = to_sq(move);
	u8 piece = pieceType(move);
	u8 cPiece = cPieceType(move);
  	
	value = see(to, cPiece, from, piece, sideToMove);
	
	return value;
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
	u64 occ     = occupied; 
	
	swapList[0] = seeVal[cPiece];
	occ ^= fromSet;
	
	u8 side = sideToMove ^ 1;
	
	u64 sideToMoveAttackers = attacksTo(occ, toSq, side);
	
	if(!sideToMoveAttackers) {
		
		return swapList[0];
	}
	
	u64 attackers = attacksTo(occ, toSq, WHITE) | attacksTo(occ, toSq, BLACK);
	
	u64 rooks = pieceBB[WHITE][ROOKS] | pieceBB[BLACK][ROOKS];
	u64 bishops = pieceBB[WHITE][BISHOPS] | pieceBB[BLACK][BISHOPS];
	u64 queens = pieceBB[WHITE][QUEEN] | pieceBB[BLACK][QUEEN];
	
	cPiece = aPiece;
	
	do {	
		for (aPiece = PAWNS; !(fromSet = sideToMoveAttackers & pieceBB[side][aPiece]); aPiece++) {
			assert(aPiece < KING);
		}
			
		fromSet &= -fromSet;
		
		occ ^= fromSet;
		attackers |= ((rooks | queens) & Rmagic(toSq, occ)) | ((bishops | queens) & Bmagic(toSq, occ));
		attackers &= occ;
	
		swapList[index] = -swapList[index - 1] + seeVal[cPiece];
			
		cPiece = aPiece - 1;
		
		index++;
		
		side ^= 1;
		
		sideToMoveAttackers = attackers & pieceBB[side][PIECES];
		
		// Stop after a king capture
		if (aPiece - 1 == KING && sideToMoveAttackers) {
			swapList[index++] = seeVal[KING];
			break;
		}
	} while(sideToMoveAttackers);
	
	while (--index) {
		if(-swapList[index] < swapList[index - 1]) {
			swapList[index - 1] = -swapList[index];
		}
	}

   return swapList[0];
}

u64 considerXrays(u64 occ, u8 square) {
	
	u64 attacks = 0ULL;
	
	if(occ) {
		attacks |= (((index_bb[square] << 7) & NOT_H_FILE) | ((index_bb[square] << 9) & NOT_A_FILE)) & pieceBB[BLACK][PAWNS];
		
		attacks |= (((index_bb[square] >> 7) & NOT_A_FILE) | ((index_bb[square] >> 9) & NOT_H_FILE)) & pieceBB[WHITE][PAWNS];
		
		/* check if a bishop is attacking a square */
		attacks |= Bmagic(square, occ) & (pieceBB[WHITE][BISHOPS] | pieceBB[BLACK][BISHOPS] | pieceBB[WHITE][QUEEN] | pieceBB[BLACK][QUEEN]);
	
		/* check if a rook is attacking a square */
		attacks |= Rmagic(square, occ) & (pieceBB[WHITE][ROOKS] | pieceBB[BLACK][ROOKS] | pieceBB[WHITE][QUEEN] | pieceBB[BLACK][QUEEN]);
	}
	return attacks;
}

u64 attacksTo(u64 occ, u8 square, u8 sideToMove) {
	
	 u64 attacks = 0ULL;   
	 
		if(sideToMove) {
			attacks |= (((index_bb[square] << 7) & NOT_H_FILE) | ((index_bb[square] << 9) & NOT_A_FILE)) 
					& pieceBB[BLACK][PAWNS];
		} else {
			attacks |= (((index_bb[square] >> 7) & NOT_A_FILE) | ((index_bb[square] >> 9) & NOT_H_FILE)) 
					& pieceBB[WHITE][PAWNS];
		}
		
		/* check if a knight is attacking a square */
		attacks |= get_knight_attacks(square) & pieceBB[sideToMove][KNIGHTS];
		
		/* check if a bishop or queen is attacking a square */
		attacks |= Bmagic(square, occ) & (pieceBB[sideToMove][BISHOPS] | pieceBB[sideToMove][QUEEN]);
	
		/* check if a rook or queen is attacking a square */
		attacks |= Rmagic(square, occ) & (pieceBB[sideToMove][ROOKS] | pieceBB[sideToMove][QUEEN]);
    
		/* check if a king is attacking a sq */
		attacks |= get_king_attacks(square) & pieceBB[sideToMove][KING];
	
	return attacks;
}

u64 getLeastValuablePiece(u64 attadef, u8 sideToMove, u8 *piece) {
	
	for(*piece = PAWNS; *piece <= KING; *piece += 1) {
		
		u64 subset = attadef & pieceBB[sideToMove][*piece];
		if(subset) {
			return subset & -subset;
		}
	}
   
   return 0; // empty set
}

void makeCapture(u8 piece, u8 cPiece, u8 sideToMove, u8 from, u8 to) {
	
	u64 from_bb = 1ULL << from;
	u64 to_bb = 1ULL << to;
	u64 from_to_bb = from_bb | to_bb;
	
	pieceBB[sideToMove][piece] ^= from_to_bb;
    pieceBB[sideToMove][PIECES] ^= from_to_bb;
    pieceBB[sideToMove ^ 1][cPiece] ^= to_bb;
    pieceBB[sideToMove ^ 1][PIECES] ^= to_bb;
            
    occupied ^= from_bb;
	empty ^= from_bb;        
}

void unMakeCapture(u8 piece, u8 cPiece, u8 sideToMove, u8 from, u8 to) {

	u64 from_bb = 1ULL << from;
	u64 to_bb = 1ULL << to;
	u64 from_to_bb = from_bb | to_bb;
	
    pieceBB[sideToMove][piece] ^= from_to_bb;
    pieceBB[sideToMove][PIECES] ^= from_to_bb;
            
    pieceBB[sideToMove ^ 1][cPiece] ^= to_bb;
    pieceBB[sideToMove ^ 1][PIECES] ^= to_bb;
            
    occupied ^= from_bb;
    empty ^= from_bb;
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
    for(int k = 0; k < numberOfMoves; k++) {
        
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

int ProbeHash(u8 sideToMove, int depth, int alpha, int beta, Move *bestMove) {
    
    u64 key = hashKey;
    
    if(sideToMove) {
        key ^= KEY_BLACK_TO_MOVE;
    }
    
    HASHE *phashe = &hashTable[key % HASH_TABLE_SIZE];
    
    if (phashe->key == key) {
        
        if (phashe->depth >= depth) {
            
            if (phashe->flags == hashfEXACT) {
               
			   return phashe->value;
            }
            
            if ((phashe->flags == hashfALPHA) && (phashe->value <= alpha)) {
                
                return alpha;
            }
            
            if ((phashe->flags == hashfBETA) && (phashe->value >= beta)) {
                
                return beta;
            }
		}
		
		Move move;
		move.move = phashe->move;
		move.score = 0;
	
		*bestMove = move;
    }
	    
    return VAL_UNKNOWN;
}

void RecordHash(u8 sideToMove, int depth, int value, int hashf, Move bestMove) {
    
    u64 key = hashKey;
    
    if(sideToMove) {
        key ^= KEY_BLACK_TO_MOVE;
    }
	
    HASHE *phashe = &hashTable[key % HASH_TABLE_SIZE];
    
    phashe->key = key;
    phashe->move = bestMove.move;
	phashe->value = value;
    phashe->flags = hashf;
    phashe->depth = depth;
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
 




