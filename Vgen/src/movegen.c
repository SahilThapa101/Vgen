//
//  movegen.c
//  Vgen
//
//  Created by Amar Thapa on 2/05/17.
//  Copyright © 2018 Amar Thapa. All rights reserved.
//

#include "globals.h"
#include "movegen.h"
#include "magicmoves.h"
#include "nonslidingmoves.h"
#include "utility.h"
#include "search.h"

typedef unsigned char u8;

u8 genMoves(Move *moveList, u8 sideToMove) {
    
    u32 pos = 0;
    
    pos = genAttacks(moveList, pos, sideToMove);
	pos = genPushes(moveList, pos, sideToMove);
    pos = genSpecialMoves(moveList, pos, sideToMove);
	
    return pos;
}

u8 genSpecialMoves(Move *moveList, u8 pos, u8 sideToMove) {
	
    pos = genDoublePushes(moveList, pos, sideToMove);  
	pos = genPromotionsNormal(moveList, pos, sideToMove);
    pos = genCastlingMoves(moveList, pos, sideToMove);
	
	return pos;
}

u8 genPushes(Move *moveList, u8 pos, u8 sideToMove) {
	
	int start = pos;
	
    pos = genKnightPushes(moveList, pos, sideToMove);
    pos = genBishopPushes(moveList, pos, sideToMove);
	pos = genRookPushes(moveList, pos, sideToMove);
    pos = genQueenPushes(moveList, pos, sideToMove);
    pos = genPawnPushes(moveList, pos, sideToMove);
	pos = genKingPushes(moveList, pos, sideToMove);

	return pos;
}

int pieceVal[8] = { 0, 100, 300, 300, 500, 900, 2000, 0};
u8 genAttacks(Move *moveList, u8 pos, u8 sideToMove) {
    
	int start = pos;
	
	pos = genPromotionsAttacks(moveList, pos, sideToMove);
	pos = genEnpassantMoves(moveList, pos, sideToMove);
	pos = genPawnAttacks(moveList, pos, sideToMove);
	pos = genKnightAttacks(moveList, pos, sideToMove);
    pos = genBishopAttacks(moveList, pos, sideToMove);
    pos = genRookAttacks(moveList, pos, sideToMove);
    pos = genQueenAttacks(moveList, pos, sideToMove);
    pos = genKingAttacks(moveList, pos, sideToMove);
    
	u8 aPiece;
	u8 cPiece;
	for(int i = 0; i < pos; i++) {
		
		aPiece = pieceType(moveList[i].move);
		cPiece = cPieceType(moveList[i].move);

		if(pieceVal[aPiece] >= pieceVal[cPiece]) {
			
			moveList[i].score = seeCapture(moveList[i].move, sideToMove);
		} else {
			
			moveList[i].score = pieceVal[cPiece] - pieceVal[aPiece];
		}
	}

	return pos;
}

u8 genAttacksQuies(Move *moveList, u8 sideToMove) {
    
	u8 pos = 0;

	pos = genPromotionsAttacks(moveList, pos, sideToMove);
	pos = genEnpassantMoves(moveList, pos, sideToMove);
	pos = genPawnAttacks(moveList, pos, sideToMove);
	pos = genKnightAttacks(moveList, pos, sideToMove);
    pos = genBishopAttacks(moveList, pos, sideToMove);
    pos = genRookAttacks(moveList, pos, sideToMove);
    pos = genQueenAttacks(moveList, pos, sideToMove);
    pos = genKingAttacks(moveList, pos, sideToMove);
    
    return pos;
}

/* pushes aka normal/quiet moves */
u8 genKingPushes(Move *moveList, u8 pos, u8 sideToMove) {
    
    u64 king_bb = pieceBB[sideToMove][KING];
    
    while (king_bb) {
        const u8 from = bitScanForward(king_bb);
        king_bb &= king_bb - 1;
        
        u64 pushes = get_king_attacks(from) & empty;
        
        while (pushes) {
            const u8 to = bitScanForward(pushes);
            pushes &= pushes - 1;
            
            moveList[pos++].move = createMove(0, 0, MOVE_NORMAL, sideToMove, DUMMY, KING, from, to);
	   }
    }
    
    return pos;
}

u8 genQueenPushes(Move *moveList, u8 pos, u8 sideToMove) {
    
    u64 queen_bb = pieceBB[sideToMove][QUEEN];
    
    while (queen_bb) {
        const u8 from = bitScanForward(queen_bb);
        queen_bb &= queen_bb - 1;
        
        u64 pushes = Qmagic(from, occupied) & empty;
        
        while (pushes) {
            const u8 to = bitScanForward(pushes);
            pushes &= pushes - 1;
            
            moveList[pos++].move = createMove(0, 0, MOVE_NORMAL, sideToMove, DUMMY, QUEEN, from, to);
        }
    }
    
    return pos;
}

u8 genBishopPushes(Move *moveList, u8 pos, u8 sideToMove) {
    u64 bishops_bb = pieceBB[sideToMove][BISHOPS];
    
    while (bishops_bb) {
        const u8 from = bitScanForward(bishops_bb);
        bishops_bb &= bishops_bb - 1;
        
        u64 pushes = Bmagic(from, occupied) & empty;
        
        while (pushes) {
            const u8 to = bitScanForward(pushes);
            pushes &= pushes - 1;
            
            moveList[pos++].move = createMove(0, 0, MOVE_NORMAL, sideToMove, DUMMY, BISHOPS, from, to);
        }
    }
    
    return pos;
}

u8 genKnightPushes(Move *moveList, u8 pos, u8 sideToMove) {
    
    u64 knights_bb = pieceBB[sideToMove][KNIGHTS];
    
    while (knights_bb) {
        const u8 from = bitScanForward(knights_bb);
        knights_bb &= knights_bb - 1;
        
        u64 pushes = get_knight_attacks(from) & empty;
        
        while (pushes) {
            const u8 to = bitScanForward(pushes);
            pushes &= pushes - 1;
            
            moveList[pos++].move = createMove(0, 0, MOVE_NORMAL, sideToMove, DUMMY, KNIGHTS, from, to);
        }
    }
    
    return pos;
}

u8 genRookPushes(Move *moveList, u8 pos, u8 sideToMove) {
    u64 rooks_bb = pieceBB[sideToMove][ROOKS];
    
    while (rooks_bb) {
        const u8 from = bitScanForward(rooks_bb);
        rooks_bb &= rooks_bb - 1;
        
        u64 pushes = Rmagic(from, occupied) & empty;
        
        while (pushes) {
            const u8 to = bitScanForward(pushes);
            pushes &= pushes - 1;
            
            moveList[pos++].move = createMove(0, 0, MOVE_NORMAL, sideToMove, DUMMY, ROOKS, from, to);
        }
    }
    
    return pos;
}

u8 genPawnPushes(Move *moveList, u8 pos, u8 sideToMove) {
    
    u8 from;
    u8 to;
    u64 fromBB;
    u64 toBB;
    u64 pawnsBB = pieceBB[sideToMove][PAWNS];
    u64 targetSquares;
    
    if (sideToMove == WHITE) {
        
        pawnsBB &= NOT_RANK_7;
        targetSquares  =  (pawnsBB << 8) & empty;
    } else {
        
        pawnsBB &= NOT_RANK_2;
        targetSquares  =  (pawnsBB >> 8) & empty;
    }
    
    while (targetSquares) {
        
        to = bitScanForward(targetSquares);
        
        toBB = getBitboardFromSquare(to);
        
        if (sideToMove == WHITE) {
            fromBB = toBB >> 8;
        } else {
            fromBB = toBB << 8;
        }
        
        from = bitScanForward(fromBB);
        
        moveList[pos++].move = createMove(0, 0, MOVE_NORMAL, sideToMove, DUMMY, PAWNS, from, to);
        
        targetSquares &= targetSquares - 1;
    }

    return pos;
}

u8 genDoublePushes(Move *moveList, u8 pos, u8 sideToMove) {
    
    u64 pawns_bb = pieceBB[sideToMove][PAWNS];
    
    if (sideToMove) {
		
        pawns_bb &= RANK_7;
    } else {
        pawns_bb &= RANK_2;
	}
	
    u64 pawns_single_push = ((pawns_bb << 8) >> (16 * sideToMove)) & empty;
    
    u64 pawns_double_push_target_squares = ((pawns_single_push << 8) >> (16 * sideToMove)) & empty;
    
    u64 pawns_can_push = (pawns_double_push_target_squares >> 16) << (32 * sideToMove);
    
    while (pawns_can_push) {
        const u8 from = bitScanForward(pawns_can_push);
        pawns_can_push &= pawns_can_push - 1;
        
        const u8 to = bitScanForward(pawns_double_push_target_squares);
        pawns_double_push_target_squares &= pawns_double_push_target_squares - 1;
        
        moveList[pos++].move = createMove(0, 0, MOVE_DOUBLE_PUSH, sideToMove, DUMMY, PAWNS, from, to);
    }
    
    return pos;
}

/* attacks */

u8 genKingAttacks(Move *moveList, u8 pos, u8 sideToMove) {
    u64 king_bb = pieceBB[sideToMove][KING];
    
    while (king_bb) {
        const u8 from = bitScanForward(king_bb);
        king_bb &= king_bb - 1;
        
        u64 attacks = get_king_attacks(from) & pieceBB[sideToMove ^ 1][PIECES];
        
        while (attacks) {
            
            const u8 to = bitScanForward(attacks);
            attacks &= attacks - 1;
            
            if (index_bb[to] & pieceBB[sideToMove ^ 1][QUEEN]) {
                moveList[pos++].move = createMove(0, 0, MOVE_CAPTURE, sideToMove, QUEEN, KING, from, to);
            }
            
            if (pieceBB[sideToMove ^ 1][BISHOPS]) {
                u64 bishops_bb = pieceBB[sideToMove ^ 1][BISHOPS];
                while (bishops_bb) {
                    const u8 sq = bitScanForward(bishops_bb);
                    bishops_bb &= bishops_bb - 1;
                    
                    if (sq == to)
                        moveList[pos++].move = createMove(0, 0, MOVE_CAPTURE, sideToMove, BISHOPS, KING, from, to);
                }
            }
            
            if (pieceBB[sideToMove ^ 1][KNIGHTS]) {
                u64 knights_bb = pieceBB[sideToMove ^ 1][KNIGHTS];
                while (knights_bb) {
                    const u8 sq = bitScanForward(knights_bb);
                    knights_bb &= knights_bb - 1;
                    
                    if (sq == to)
                        moveList[pos++].move = createMove(0, 0, MOVE_CAPTURE, sideToMove, KNIGHTS, KING, from, to);
                }
            }
            
            if (pieceBB[sideToMove ^ 1][ROOKS]) {
                u64 rooks_bb = pieceBB[sideToMove ^ 1][ROOKS];
                while (rooks_bb) {
                    const u8 sq = bitScanForward(rooks_bb);
                    rooks_bb &= rooks_bb - 1;
                    
                    if (sq == to)
                        moveList[pos++].move = createMove(0, 0, MOVE_CAPTURE, sideToMove, ROOKS, KING, from, to);
                }
            }
            
            if (pieceBB[sideToMove ^ 1][PAWNS]) {
                u64 pawns_bb = pieceBB[sideToMove ^ 1][PAWNS];
                while (pawns_bb) {
                    const u8 sq = bitScanForward(pawns_bb);
                    pawns_bb &= pawns_bb - 1;
                    
                    if (sq == to)
                        moveList[pos++].move = createMove(0, 0, MOVE_CAPTURE, sideToMove, PAWNS, KING, from, to);
                }
            }
        }
    }
    
    return pos;
}

u8 genQueenAttacks(Move *moveList, u8 pos, u8 sideToMove) {
    u64 queen_bb = pieceBB[sideToMove][QUEEN];
    while (queen_bb) {
        const u8 from = bitScanForward(queen_bb);
        queen_bb &= queen_bb - 1;
        
        u64 attacks = Qmagic(from, occupied) & pieceBB[sideToMove ^ 1][PIECES];
        
        while (attacks) {
            const u8 to = bitScanForward(attacks);
            attacks &= attacks - 1;
            
            if (index_bb[to] & pieceBB[sideToMove ^ 1][QUEEN]) {
                moveList[pos++].move = createMove(0, 0, MOVE_CAPTURE, sideToMove, QUEEN, QUEEN, from, to);
            }
            
            if (pieceBB[sideToMove ^ 1][BISHOPS]) {
                u64 bishops_bb = pieceBB[sideToMove ^ 1][BISHOPS];
                while (bishops_bb) {
                    const u8 sq = bitScanForward(bishops_bb);
                    bishops_bb &= bishops_bb - 1;
                    
                    if (sq == to)
                        moveList[pos++].move = createMove(0, 0, MOVE_CAPTURE, sideToMove, BISHOPS, QUEEN, from, to);
                }
            }
            
            if (pieceBB[sideToMove ^ 1][KNIGHTS]) {
                u64 knights_bb = pieceBB[sideToMove ^ 1][KNIGHTS];
                while (knights_bb) {
                    const u8 sq = bitScanForward(knights_bb);
                    knights_bb &= knights_bb - 1;
                    
                    if (sq == to)
                        moveList[pos++].move = createMove(0, 0, MOVE_CAPTURE, sideToMove, KNIGHTS, QUEEN, from, to);
                }
            }
            
            if (pieceBB[sideToMove ^ 1][ROOKS]) {
                u64 rooks_bb = pieceBB[sideToMove ^ 1][ROOKS];
                while (rooks_bb) {
                    const u8 sq = bitScanForward(rooks_bb);
                    rooks_bb &= rooks_bb - 1;
                    
                    if (sq == to)
                        moveList[pos++].move = createMove(0, 0, MOVE_CAPTURE, sideToMove, ROOKS, QUEEN, from, to);
                }
            }
            
            if (pieceBB[sideToMove ^ 1][PAWNS]) {
                u64 pawns_bb = pieceBB[sideToMove ^ 1][PAWNS];
                while (pawns_bb) {
                    const u8 sq = bitScanForward(pawns_bb);
                    pawns_bb &= pawns_bb - 1;
                    
                    if (sq == to)
                        moveList[pos++].move = createMove(0, 0, MOVE_CAPTURE, sideToMove, PAWNS, QUEEN, from, to);
                }
            }
        }
    }
    return pos;
}

u8 genBishopAttacks(Move *moveList, u8 pos, u8 sideToMove) {
    u64 bishop_bb = pieceBB[sideToMove][BISHOPS];
    
    while (bishop_bb) {
        const u8 from = bitScanForward(bishop_bb);
        bishop_bb &= bishop_bb - 1;
        
        u64 attacks = Bmagic(from, occupied) & (pieceBB[sideToMove ^ 1][PIECES]);
        
        while (attacks) {
            
            const u8 to = bitScanForward(attacks);
            attacks &= attacks - 1;
            
            if (index_bb[to] & pieceBB[sideToMove ^ 1][QUEEN]) {
                moveList[pos++].move = createMove(0, 0, MOVE_CAPTURE, sideToMove, QUEEN, BISHOPS, from, to);
            }
            
            if (pieceBB[sideToMove ^ 1][BISHOPS]) {
                u64 bishops_bb = pieceBB[sideToMove ^ 1][BISHOPS];
                while (bishops_bb) {
                    const u8 sq = bitScanForward(bishops_bb);
                    bishops_bb &= bishops_bb - 1;
                    
                    if (sq == to)
                        moveList[pos++].move = createMove(0, 0, MOVE_CAPTURE, sideToMove, BISHOPS, BISHOPS, from, to);
                }
            }
            
            if (pieceBB[sideToMove ^ 1][KNIGHTS]) {
                u64 knights_bb = pieceBB[sideToMove ^ 1][KNIGHTS];
                while (knights_bb) {
                    const u8 sq = bitScanForward(knights_bb);
                    knights_bb &= knights_bb - 1;
                    
                    if (sq == to)
                        moveList[pos++].move = createMove(0, 0, MOVE_CAPTURE, sideToMove, KNIGHTS, BISHOPS, from, to);
                }
            }
            
            if (pieceBB[sideToMove ^ 1][ROOKS]) {
                u64 rooks_bb = pieceBB[sideToMove ^ 1][ROOKS];
                while (rooks_bb) {
                    const u8 sq = bitScanForward(rooks_bb);
                    rooks_bb &= rooks_bb - 1;
                    
                    if (sq == to)
                        moveList[pos++].move = createMove(0, 0, MOVE_CAPTURE, sideToMove, ROOKS, BISHOPS, from, to);
                }
            }
            
            if (pieceBB[sideToMove ^ 1][PAWNS]) {
                u64 pawns_bb = pieceBB[sideToMove ^ 1][PAWNS];
                while (pawns_bb) {
                    const u8 sq = bitScanForward(pawns_bb);
                    pawns_bb &= pawns_bb - 1;
                    
                    if (sq == to)
                        moveList[pos++].move = createMove(0, 0, MOVE_CAPTURE, sideToMove, PAWNS, BISHOPS, from, to);
                }
            }
        }
    }
    
    return pos;
}

u8 genKnightAttacks(Move *moveList, u8 pos, u8 sideToMove) {
    
    u64 knights_bb = pieceBB[sideToMove][KNIGHTS];
    
    while (knights_bb) {
        const u8 from = bitScanForward(knights_bb);
        knights_bb &= knights_bb - 1;
        
        u64 attacks = get_knight_attacks(from) & (pieceBB[sideToMove ^ 1][PIECES]);
        
        while (attacks) {
            
            const u8 to = bitScanForward(attacks);
            attacks &= attacks - 1;
            
			u64 queen_bb = pieceBB[sideToMove ^ 1][QUEEN];
            while (queen_bb) {
                const u8 sq = bitScanForward(queen_bb);
                queen_bb &= queen_bb - 1;
                    
                if (sq == to) {
                    moveList[pos++].move = createMove(0, 0, MOVE_CAPTURE, sideToMove, QUEEN, KNIGHTS, from, to);
				}
			}
            
            u64 bishops_bb = pieceBB[sideToMove ^ 1][BISHOPS];
            while (bishops_bb) {
                const u8 sq = bitScanForward(bishops_bb);
                bishops_bb &= bishops_bb - 1;
                    
                if (sq == to) {
                    moveList[pos++].move = createMove(0, 0, MOVE_CAPTURE, sideToMove, BISHOPS, KNIGHTS, from, to);
				}
			}
            
            u64 knights_bb = pieceBB[sideToMove ^ 1][KNIGHTS];
			while (knights_bb) {
                const u8 sq = bitScanForward(knights_bb);
                knights_bb &= knights_bb - 1;
                    
                if (sq == to) {
                    moveList[pos++].move = createMove(0, 0, MOVE_CAPTURE, sideToMove, KNIGHTS, KNIGHTS, from, to);
				}
			}
            
            u64 rooks_bb = pieceBB[sideToMove ^ 1][ROOKS];
            while (rooks_bb) {
                const u8 sq = bitScanForward(rooks_bb);
                rooks_bb &= rooks_bb - 1;
                    
                if (sq == to) {
				
					moveList[pos++].move = createMove(0, 0, MOVE_CAPTURE, sideToMove, ROOKS, KNIGHTS, from, to);
				}
			}
            
            u64 pawns_bb = pieceBB[sideToMove ^ 1][PAWNS];
            while (pawns_bb) {
                const u8 sq = bitScanForward(pawns_bb);
                pawns_bb &= pawns_bb - 1;
                    
                if (sq == to) {
                    
                    moveList[pos++].move = createMove(0, 0, MOVE_CAPTURE, sideToMove, PAWNS, KNIGHTS, from, to);
                }
            }
        }
    }
    
    return pos;
}

u8 genRookAttacks(Move *moveList, u8 pos, u8 sideToMove) {
    u64 rooks_bb = pieceBB[sideToMove][ROOKS];
    while (rooks_bb) {
        const u8 from = bitScanForward(rooks_bb);
        rooks_bb &= rooks_bb - 1;
        u64 attacks = Rmagic(from, occupied) & (pieceBB[sideToMove ^ 1][PIECES]);
        
        while (attacks) {
            
            const u8 to = bitScanForward(attacks);
            attacks &= attacks - 1;
            
            if (index_bb[to] & pieceBB[sideToMove ^ 1][QUEEN]) {
                moveList[pos++].move = createMove(0, 0, MOVE_CAPTURE, sideToMove, QUEEN, ROOKS, from, to);
            }
            
            if (pieceBB[sideToMove ^ 1][BISHOPS]) {
                u64 bishops_bb = pieceBB[sideToMove ^ 1][BISHOPS];
                while (bishops_bb) {
                    const u8 sq = bitScanForward(bishops_bb);
                    bishops_bb &= bishops_bb - 1;
                    
                    if (sq == to) {
                        moveList[pos++].move = createMove(0, 0, MOVE_CAPTURE, sideToMove, BISHOPS, ROOKS, from, to);
                    }
                }
            }
            
            if (pieceBB[sideToMove ^ 1][KNIGHTS]) {
                u64 knights_bb = pieceBB[sideToMove ^ 1][KNIGHTS];
                while (knights_bb) {
                    const u8 sq = bitScanForward(knights_bb);
                    knights_bb &= knights_bb - 1;
                    
                    if (sq == to)
                        moveList[pos++].move = createMove(0, 0, MOVE_CAPTURE, sideToMove, KNIGHTS, ROOKS, from, to);
                }
            }
            
            if (pieceBB[sideToMove ^ 1][ROOKS]) {
                u64 rooks_bb = pieceBB[sideToMove ^ 1][ROOKS];
                while (rooks_bb) {
                    const u8 sq = bitScanForward(rooks_bb);
                    rooks_bb &= rooks_bb - 1;
                    
                    if (sq == to)
                        moveList[pos++].move = createMove(0, 0, MOVE_CAPTURE, sideToMove, ROOKS, ROOKS, from, to);
                }
            }
            
            if (pieceBB[sideToMove ^ 1][PAWNS]) {
                u64 pawns_bb = pieceBB[sideToMove ^ 1][PAWNS];
                
                while (pawns_bb) {
                    const u8 sq = bitScanForward(pawns_bb);
                    pawns_bb &= pawns_bb - 1;
                    
                    if (sq == to) {
                        
                        moveList[pos++].move = createMove(0, 0, MOVE_CAPTURE, sideToMove, PAWNS, ROOKS, from, to);
                    }
                }
            }
        }
    }
    
    return pos;
}

u8 genPawnAttacks(Move *moveList, u8 pos, u8 sideToMove) {
    u64 pawnsBB = pieceBB[sideToMove][PAWNS];
    
    // generation of promotion moves handles the attack for pawns at Rank 2 and 7
    if (sideToMove == WHITE) {
        
        pawnsBB &= NOT_RANK_7;
    } else {
        
        pawnsBB &= NOT_RANK_2;
    }
    
    while (pawnsBB) {
        const u8 from = bitScanForward(pawnsBB);
        pawnsBB &= pawnsBB - 1;
        
        u64 attacks;
        
        if (sideToMove) {
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
            
            if (pieceBB[sideToMove ^ 1][PAWNS]) {
                u64 pawns_bb = pieceBB[sideToMove ^ 1][PAWNS];
                while (pawns_bb) {
                    const u8 sq = bitScanForward(pawns_bb);
                    pawns_bb &= pawns_bb - 1;
                    
                    if (sq == to) {
                        moveList[pos++].move = createMove(0, 0, MOVE_CAPTURE, sideToMove, PAWNS, PAWNS, from, to);
                    }
                }
            }
            
            if (index_bb[to] & pieceBB[sideToMove ^ 1][QUEEN]) {
                moveList[pos++].move = createMove(0, 0, MOVE_CAPTURE, sideToMove, QUEEN, PAWNS, from, to);
            }
            
            if (pieceBB[sideToMove ^ 1][BISHOPS]) {
                u64 bishops_bb = pieceBB[sideToMove ^ 1][BISHOPS];
                while (bishops_bb) {
                    const u8 sq = bitScanForward(bishops_bb);
                    bishops_bb &= bishops_bb - 1;
                    
                    if (sq == to)
                        moveList[pos++].move = createMove(0, 0, MOVE_CAPTURE, sideToMove, BISHOPS, PAWNS, from, to);
                }
            }
            
            if (pieceBB[sideToMove ^ 1][KNIGHTS]) {
                u64 knights_bb = pieceBB[sideToMove ^ 1][KNIGHTS];
                while (knights_bb) {
                    const u8 sq = bitScanForward(knights_bb);
                    knights_bb &= knights_bb - 1;
                    
                    if (sq == to)
                        moveList[pos++].move = createMove(0, 0, MOVE_CAPTURE, sideToMove, KNIGHTS, PAWNS, from, to);
                }
            }
            
            if (pieceBB[sideToMove ^ 1][ROOKS]) {
                u64 rooks_bb = pieceBB[sideToMove ^ 1][ROOKS];
                while (rooks_bb) {
                    const u8 sq = bitScanForward(rooks_bb);
                    rooks_bb &= rooks_bb - 1;
                    
                    if (sq == to)
                        moveList[pos++].move = createMove(0, 0, MOVE_CAPTURE, sideToMove, ROOKS, PAWNS, from, to);
                }
            }
        }
    }
    
    return pos;
}

u8 genCastlingMoves(Move *moveList, u8 pos, u8 sideToMove) {
    
    if (sideToMove == WHITE) {
        
        if (moveStack[ply].castleFlags & CastleFlagWhiteQueen) {
            u64 wq_sqs = empty & WQ_SIDE_SQS;
            if (wq_sqs == WQ_SIDE_SQS
                && !(isSqAttacked(2, WHITE) || isSqAttacked(3, WHITE)
                     || isSqAttacked(4, WHITE))) {
                    
                    u32 move = createMove(0, 0, MOVE_CASTLE, WHITE, ROOKS, KING, 4, 2);
                    moveList[pos++].move = move;
            }
        }
        
        if (moveStack[ply].castleFlags & CastleFlagWhiteKing) {
            
            u64 wk_sqs = empty & WK_SIDE_SQS;
            
            if (wk_sqs == WK_SIDE_SQS
                && !(isSqAttacked(4, WHITE) || isSqAttacked(5, WHITE)
                     || isSqAttacked(6, WHITE))) {
                    u32 move = createMove(0, 1, MOVE_CASTLE, WHITE, ROOKS, KING, 4, 6);
                    moveList[pos++].move = move;
                }
        }
    } else {
        
        if (moveStack[ply].castleFlags & CastleFlagBlackQueen) {
            u64 bq_sqs = empty & BQ_SIDE_SQS;
            
            if (bq_sqs == BQ_SIDE_SQS
                && !(isSqAttacked(58, BLACK) || isSqAttacked(59, BLACK)
                     || isSqAttacked(60, BLACK))) {
                    
                    u32 move = createMove(0, 2, MOVE_CASTLE, BLACK, ROOKS, KING, 60, 58);
                    moveList[pos++].move = move;
                }
            
        }
        
        if (moveStack[ply].castleFlags & CastleFlagBlackKing) {
            
            u64 bk_sqs = empty & BK_SIDE_SQS;
            
            if (bk_sqs == BK_SIDE_SQS
                && !(isSqAttacked(60, BLACK) || isSqAttacked(61, BLACK)
                     || isSqAttacked(62, BLACK))) {
                    u32 move = createMove(0, 3, MOVE_CASTLE, BLACK, ROOKS, KING, 60, 62);
                    moveList[pos++].move = move;
                }
        }
    }
    
    return pos;
}

u8 genEnpassantMoves(Move *moveList, u8 pos, u8 sideToMove) {
    
    if (moveStack[ply].epFlag) {
        
        u8 from;
        u8 to;
        u32 move;
        u64 target_sqs;
        u64 target_pawns;
        u64 pawns = pieceBB[sideToMove][PAWNS];
        u64 epSquare = moveStack[ply].epSquare;
        
        if (sideToMove == WHITE) {
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
            
            move = createMove(0, 0, MOVE_ENPASSANT, sideToMove, PAWNS, PAWNS, from, to);
            moveList[pos++].move = move;
            
        }
    }
    
    return pos;
}

u8 genPromotionsNormal(Move *moveList, u8 pos, u8 sideToMove) {
    
	u8 sq;
    u8 from;
    u8 to;
	u64 toPush;
    u64 fromBB;
	u64 pawns = pieceBB[sideToMove][PAWNS];
    u64 pawnsToPromote;
    
    if (sideToMove == WHITE) {
        
        pawnsToPromote = pawns & RANK_7;
    } else {
        
        pawnsToPromote = pawns & RANK_2;
    }

	while (pawnsToPromote) {
        
        from = bitScanForward(pawnsToPromote);
        fromBB = getBitboardFromSquare(from);
        
        if (sideToMove == WHITE) {
            
            toPush = (fromBB << 8) & empty;
        } else {
            
            toPush = (fromBB >> 8) & empty;
        }
        
        while (toPush) {
            to = bitScanForward(toPush);
            toPush &= toPush - 1;
            
            moveList[pos++].move = createMove(PROMOTE_TO_QUEEN, 0, MOVE_PROMOTION, sideToMove, DUMMY, PAWNS, from, to);
            moveList[pos++].move = createMove(PROMOTE_TO_ROOK, 0, MOVE_PROMOTION, sideToMove, DUMMY, PAWNS, from, to);
            moveList[pos++].move = createMove(PROMOTE_TO_BISHOP, 0, MOVE_PROMOTION, sideToMove, DUMMY, PAWNS, from, to);
            moveList[pos++].move = createMove(PROMOTE_TO_KNIGHT, 0, MOVE_PROMOTION, sideToMove, DUMMY, PAWNS, from, to);
        }
		
        pawnsToPromote &= pawnsToPromote - 1;
    }
    
    return pos;
}

u8 genPromotionsAttacks(Move *moveList, u8 pos, u8 sideToMove) {
    
    u8 sq;
    u8 from;
    u8 to;
    u64 toAttack;
    u64 fromBB;
    u64 pawns = pieceBB[sideToMove][PAWNS];
    u64 pawnsToPromote;
    u64 queenBB;
    u64 rooksBB;
    u64 bishopsBB;
    u64 knightsBB;
    
    if (sideToMove == WHITE) {
        
        pawnsToPromote = pawns & RANK_7;
    } else {
        
        pawnsToPromote = pawns & RANK_2;
    }
    
    while (pawnsToPromote) {
        
        from = bitScanForward(pawnsToPromote);
        fromBB = getBitboardFromSquare(from);
        
        if (sideToMove == WHITE) {
            
            toAttack = (((fromBB << 7) & NOT_H_FILE) | ((fromBB << 9) & NOT_A_FILE));
        } else {
            
            toAttack = (((fromBB >> 7) & NOT_A_FILE) | ((fromBB >> 9) & NOT_H_FILE));
        }
        
        while (toAttack) {
            
            to = bitScanForward(toAttack);
            toAttack &= toAttack - 1;
            
            if (pieceBB[sideToMove ^ 1][QUEEN]) {
                
                queenBB = pieceBB[sideToMove ^ 1][QUEEN];
                
                while (queenBB) {
                    
                    sq = bitScanForward(queenBB);
                    queenBB &= queenBB - 1;
                    
                    if (sq == to)  {
                        moveList[pos++].move = createMove(PROMOTE_TO_QUEEN, 0, MOVE_PROMOTION, sideToMove, QUEEN, PAWNS, from, to);
                        moveList[pos++].move = createMove(PROMOTE_TO_ROOK, 0, MOVE_PROMOTION, sideToMove, QUEEN, PAWNS, from, to);
                        moveList[pos++].move = createMove(PROMOTE_TO_BISHOP, 0, MOVE_PROMOTION, sideToMove, QUEEN, PAWNS, from, to);
                        moveList[pos++].move = createMove(PROMOTE_TO_KNIGHT, 0, MOVE_PROMOTION, sideToMove, QUEEN, PAWNS, from, to);
                    }
                }
            }
            
            if (pieceBB[sideToMove ^ 1][ROOKS]) {
                
                rooksBB = pieceBB[sideToMove ^ 1][ROOKS];
                
                while (rooksBB) {
                    
                    sq = bitScanForward(rooksBB);
                    rooksBB &= rooksBB - 1;
                    
                    if (sq == to) {
                        
                        moveList[pos++].move = createMove(PROMOTE_TO_QUEEN, 0, MOVE_PROMOTION, sideToMove, ROOKS, PAWNS, from, to);
                        moveList[pos++].move = createMove(PROMOTE_TO_ROOK, 0, MOVE_PROMOTION, sideToMove, ROOKS, PAWNS, from, to);
                        moveList[pos++].move = createMove(PROMOTE_TO_BISHOP, 0, MOVE_PROMOTION, sideToMove, ROOKS, PAWNS, from, to);
                        moveList[pos++].move = createMove(PROMOTE_TO_KNIGHT, 0, MOVE_PROMOTION, sideToMove, ROOKS, PAWNS, from, to);
                    }
                }
            }
            
            if (pieceBB[sideToMove ^ 1][BISHOPS]) {
                
                bishopsBB = pieceBB[sideToMove ^ 1][BISHOPS];
                
                while (bishopsBB) {
                    
                    sq = bitScanForward(bishopsBB);
                    bishopsBB &= bishopsBB - 1;
                    
                    if (sq == to) {
                        
                        moveList[pos++].move = createMove(PROMOTE_TO_QUEEN, 0, MOVE_PROMOTION, sideToMove, BISHOPS, PAWNS, from, to);
                        moveList[pos++].move = createMove(PROMOTE_TO_ROOK, 0, MOVE_PROMOTION, sideToMove, BISHOPS, PAWNS, from, to);
                        moveList[pos++].move = createMove(PROMOTE_TO_BISHOP, 0, MOVE_PROMOTION, sideToMove, BISHOPS, PAWNS, from, to);
                        moveList[pos++].move = createMove(PROMOTE_TO_KNIGHT, 0, MOVE_PROMOTION, sideToMove, BISHOPS, PAWNS, from, to);
                    }
                }
            }
            
            if (pieceBB[sideToMove ^ 1][KNIGHTS]) {
                
                knightsBB = pieceBB[sideToMove ^ 1][KNIGHTS];
                
                while (knightsBB) {
                    
                    sq = bitScanForward(knightsBB);
                    knightsBB &= knightsBB - 1;
                    
                    if (sq == to) {
                        
                        moveList[pos++].move = createMove(PROMOTE_TO_QUEEN, 0, MOVE_PROMOTION, sideToMove, KNIGHTS, PAWNS, from, to);
                        moveList[pos++].move = createMove(PROMOTE_TO_ROOK, 0, MOVE_PROMOTION, sideToMove, KNIGHTS, PAWNS, from, to);
                        moveList[pos++].move = createMove(PROMOTE_TO_BISHOP, 0, MOVE_PROMOTION, sideToMove, KNIGHTS, PAWNS, from, to);
                        moveList[pos++].move = createMove(PROMOTE_TO_KNIGHT, 0, MOVE_PROMOTION, sideToMove, KNIGHTS, PAWNS, from, to);
                    }
                }
            }
            
            pawnsToPromote &= pawnsToPromote - 1;
        }
    }
    
    return pos;
}

/*
 *    Move Type
 *
 *    0 -> quiet moves
 *    1 -> captures
 *    2 -> pawn double moves
 *    3 -> en_passant
 *    4 -> castling
 *    5 -> promotions
 *    6 -> king moves
 */

/*
 *    Promotion Type
 *
 *    0 - Queen
 *    1 - Rook
 *    2 - Knight
 *    3 - Bishop
 **/

u32 createMove(u8 promotion_type, u8 castleDir, u8 move_type, u8 sideToMove,
               u8 c_piece, u8 piece, u8 from, u8 to) {
    
    return (0ULL | (u32) promotion_type << 24 | (u32) castleDir << 22
            | (u32) move_type << 19 | (u32) sideToMove << 18 | (u32) c_piece << 15
            | (u32) piece << 12 | (u32) from << 6 | (u32) to);
}

/* Extract data from a move structure */
#define promType(move)           (move & 0x3000000) >> 24
#define castleDir(move)        (move & 0xC00000) >> 22
#define move_type(move)         (move & 0x380000) >> 19
#define colorType(move)        (move & 0x40000) >> 18
#define cPieceType(move)      (move & 0x38000) >> 15
#define pieceType(move)        (move & 0x7000) >> 12
#define from_sq(move)              (move & 0xFC0) >> 6
#define to_sq(move)                move & 0x3F


