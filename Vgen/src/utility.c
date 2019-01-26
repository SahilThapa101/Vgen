//
//  utility.c
//  Vgen
//
//  Created by Amar Thapa on 23/12/18.
//  Copyright © 2018 Amar Thapa. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "utility.h"
#include "nonslidingmoves.h"
#include "magicmoves.h"
#include "make_unmake.h"
#include "movegen.h"
#include "perft.h"
#include "vtime.h"

u64 rand64() {
    
    return rand() ^ ((u64)rand() << 15) ^ ((u64)rand() << 30) ^
    ((u64) rand() << 45) ^ ((u64)rand() << 60);
}

void print_bb(u64 board) {
    
    printf(" \n ---------- ");
    int row;
    int col;
    for (row = 7; row >= 0; row--) {
        printf("\n|%d ", row + 1);
        for (col = 0; col < 8; col++) {
            if ((((u64) 1) << ((row * 8 + col)) & board)
                == (u64) 1 << (row * 8 + col)) {
                putchar('x');
            } else
                putchar('.');
        }
        printf("|");
    }
    printf("\n|  abcdefgh|\n");
    printf(" ---------- \n");
}

void print_board(u64 board) {
    
    printf(" ---------- ");
    int row;
    int col;
    for (row = 7; row >= 0; row--) {
        printf("\n|%d ", row + 1);
        for (col = 0; col < 8; col++) {
            if ((((u64) 1) << ((row * 8 + col)) & board)
                == (u64) 1 << (row * 8 + col)) {
                
                if ((u64) 1 << (row * 8 + col) & pieceBB[WHITE][KING])
                    putchar('K');
                if ((u64) 1 << (row * 8 + col) & pieceBB[WHITE][QUEEN])
                    putchar('Q');
                if ((u64) 1 << (row * 8 + col) & pieceBB[WHITE][BISHOPS])
                    putchar('B');
                if ((u64) 1 << (row * 8 + col) & pieceBB[WHITE][KNIGHTS])
                    putchar('N');
                if ((u64) 1 << (row * 8 + col) & pieceBB[WHITE][ROOKS])
                    putchar('R');
                if ((u64) 1 << (row * 8 + col) & pieceBB[WHITE][PAWNS])
                    putchar('P');
                
                if ((u64) 1 << (row * 8 + col) & pieceBB[BLACK][KING])
                    putchar('k');
                if ((u64) 1 << (row * 8 + col) & pieceBB[BLACK][QUEEN])
                    putchar('q');
                if ((u64) 1 << (row * 8 + col) & pieceBB[BLACK][BISHOPS])
                    putchar('b');
                if ((u64) 1 << (row * 8 + col) & pieceBB[BLACK][KNIGHTS])
                    putchar('n');
                if ((u64) 1 << (row * 8 + col) & pieceBB[BLACK][ROOKS])
                    putchar('r');
                if ((u64) 1 << (row * 8 + col) & pieceBB[BLACK][PAWNS])
                    putchar('p');
            } else
                putchar('.');
        }
        printf("|");
    }
    printf("\n|  abcdefgh|\n");
    printf(" ---------- \n");
}


//    bitScanForward
//     @author Kim Walisch (2012)
//      @param bb bitboard to scan
//       @precondition bb != 0
//        @return index (0..63) of least significant one bit


const int index64[64] = { 0, 47, 1, 56, 48, 27, 2, 60, 57, 49, 41, 37, 28, 16,
    3, 61, 54, 58, 35, 52, 50, 42, 21, 44, 38, 32, 29, 23, 17, 11, 4, 62,
    46, 55, 26, 59, 40, 36, 15, 53, 34, 51, 20, 43, 31, 22, 10, 45, 25, 39,
    14, 33, 19, 30, 9, 24, 13, 18, 8, 12, 7, 6, 5, 63
};

int bitScanForward(u64 board) {
    const u64 debruijn64 = 285870213051386505U;
    
    return index64[((board ^ (board - 1)) * debruijn64) >> 58];
}

// for population count
const u64 k1 = C64(0x5555555555555555); /*  -1/3   */
const u64 k2 = C64(0x3333333333333333); /*  -1/5   */
const u64 k4 = C64(0x0f0f0f0f0f0f0f0f); /*  -1/17  */
const u64 kf = C64(0x0101010101010101); /*  -1/255 */

// population count
int popCount (u64 x) {
    x =  x       - ((x >> 1)  & k1); /* put count of each 2 bits into those 2 bits */
    x = (x & k2) + ((x >> 2)  & k2); /* put count of each 4 bits into those 4 bits */
    x = (x       +  (x >> 4)) & k4 ; /* put count of each 8 bits into those 8 bits */
    x = (x * kf) >> 56; /* returns 8 most significant bits of x + (x<<8) + (x<<16) + (x<<24) + ...  */
    return (int) x;
}

bool isKingInCheck(u8 sideToMove) {
    
    const u8 square = bitScanForward(pieceBB[sideToMove][KING]);
    
	// pawn attacks
    if(sideToMove) {
        if((((index_bb[square] >> 7) & NOT_A_FILE) | ((index_bb[square] >> 9) & NOT_H_FILE))
           & pieceBB[WHITE][PAWNS]) {
            return true;
        }
    } else {
        if((((index_bb[square] << 7) & NOT_H_FILE) | ((index_bb[square] << 9) & NOT_A_FILE))
           & pieceBB[BLACK][PAWNS]) {
            return true;
        }
    }
    
    if(Bmagic(square, occupied) & pieceBB[sideToMove ^ 1][BISHOPS]) {
        return true;
    } else if(Rmagic(square, occupied) & pieceBB[sideToMove ^ 1][ROOKS]) {
        return true;
    } else if(Qmagic(square, occupied) & pieceBB[sideToMove ^ 1][QUEEN]) {
        return true;
    } else if (get_knight_attacks(square) & pieceBB[sideToMove ^ 1][KNIGHTS]) {
        return true;
	} else if(get_king_attacks(square) & pieceBB[sideToMove ^ 1][KING]) {
		return true;
	}
    
    return false;
}


/* function to check if a square is attacked */

bool isSqAttacked(u8 sq, u8 color) {
    
    u64 attacks;
    
    /* check if a king is attacking a sq */
    
    attacks = get_king_attacks(sq);
    
    if (attacks & pieceBB[color ^ 1][KING])
        return true;
    
    /* check if a queen is attacking a sq */
    
    attacks = Qmagic(sq, occupied);
    
    if (attacks & pieceBB[color ^ 1][QUEEN])
        return true;
    
    /* check if a bishop is attacking a square */
    
    attacks = Bmagic(sq, occupied);
    
    if (attacks & pieceBB[color ^ 1][BISHOPS])
        return true;
    
    /* check if a knight is attacking a square */
    
    attacks = get_knight_attacks(sq);
    
    if (attacks & pieceBB[color ^ 1][KNIGHTS])
        return true;
    
    /* check if a rook is attacking a square */
    
    attacks = Rmagic(sq, occupied);
    
    if (attacks & pieceBB[color ^ 1][ROOKS])
        return true;
    
    /* check if a pawn is attacking a square */
    
    if (color == WHITE)
        attacks = ((index_bb[sq] << 7) & NOT_H_FILE)
        | ((index_bb[sq] << 9) & NOT_A_FILE);
    else
        attacks = ((index_bb[sq] >> 7) & NOT_A_FILE)
        | ((index_bb[sq] >> 9) & NOT_H_FILE);
    
    if (attacks & pieceBB[color ^ 1][PAWNS])
        return true;
    
    return false;
}

u64 getBitboardFromSquare(int sq) {
    
	return 1ULL << sq;
}

char* algebricPos(u8 sq) {
    switch (sq) {
        case 0:
            return "a1";
        case 1:
            return "b1";
        case 2:
            return "c1";
        case 3:
            return "d1";
        case 4:
            return "e1";
        case 5:
            return "f1";
        case 6:
            return "g1";
        case 7:
            return "h1";
        case 8:
            return "a2";
        case 9:
            return "b2";
        case 10:
            return "c2";
        case 11:
            return "d2";
        case 12:
            return "e2";
        case 13:
            return "f2";
        case 14:
            return "g2";
        case 15:
            return "h2";
        case 16:
            return "a3";
        case 17:
            return "b3";
        case 18:
            return "c3";
        case 19:
            return "d3";
        case 20:
            return "e3";
        case 21:
            return "f3";
        case 22:
            return "g3";
        case 23:
            return "h3";
        case 24:
            return "a4";
        case 25:
            return "b4";
        case 26:
            return "c4";
        case 27:
            return "d4";
        case 28:
            return "e4";
        case 29:
            return "f4";
        case 30:
            return "g4";
        case 31:
            return "h4";
        case 32:
            return "a5";
        case 33:
            return "b5";
        case 34:
            return "c5";
        case 35:
            return "d5";
        case 36:
            return "e5";
        case 37:
            return "f5";
        case 38:
            return "g5";
        case 39:
            return "h5";
        case 40:
            return "a6";
        case 41:
            return "b6";
        case 42:
            return "c6";
        case 43:
            return "d6";
        case 44:
            return "e6";
        case 45:
            return "f6";
        case 46:
            return "g6";
        case 47:
            return "h6";
        case 48:
            return "a7";
        case 49:
            return "b7";
        case 50:
            return "c7";
        case 51:
            return "d7";
        case 52:
            return "e7";
        case 53:
            return "f7";
        case 54:
            return "g7";
        case 55:
            return "h7";
        case 56:
            return "a8";
        case 57:
            return "b8";
        case 58:
            return "c8";
        case 59:
            return "d8";
        case 60:
            return "e8";
        case 61:
            return "f8";
        case 62:
            return "g8";
        case 63:
            return "h8";
            
        default:
            break;
    }
    
    return "";
}

u64 bbFromAlgebricPos(char* posName) {
    
    if (strcmp(posName, "a1") == 0) {
        return getBitboardFromSquare(0);
    } else if (strcmp(posName, "b1") == 0) {
        return getBitboardFromSquare(1);
    } else if (strcmp(posName, "c1") == 0) {
        return getBitboardFromSquare(2);
    } else if (strcmp(posName, "d1") == 0) {
        return getBitboardFromSquare(3);
    } else if (strcmp(posName, "e1") == 0) {
        return getBitboardFromSquare(4);
    } else if (strcmp(posName, "f1") == 0) {
        return getBitboardFromSquare(5);
    } else if (strcmp(posName, "g1") == 0) {
        return getBitboardFromSquare(6);
    } else if (strcmp(posName, "h1") == 0) {
        return getBitboardFromSquare(7);
    }
    
    if (strcmp(posName, "a2") == 0) {
        return getBitboardFromSquare(8);
    } else if (strcmp(posName, "b2") == 0) {
        return getBitboardFromSquare(9);
    } else if (strcmp(posName, "c2") == 0) {
        return getBitboardFromSquare(10);
    } else if (strcmp(posName, "d2") == 0) {
        return getBitboardFromSquare(11);
    } else if (strcmp(posName, "e2") == 0) {
        return getBitboardFromSquare(12);
    } else if (strcmp(posName, "f2") == 0) {
        return getBitboardFromSquare(13);
    } else if (strcmp(posName, "g2") == 0) {
        return getBitboardFromSquare(14);
    } else if (strcmp(posName, "h2") == 0) {
        return getBitboardFromSquare(15);
    }
    
    if (strcmp(posName, "a3") == 0) {
        return getBitboardFromSquare(16);
    } else if (strcmp(posName, "b3") == 0) {
        return getBitboardFromSquare(17);
    } else if (strcmp(posName, "c3") == 0) {
        return getBitboardFromSquare(18);
    } else if (strcmp(posName, "d3") == 0) {
        return getBitboardFromSquare(19);
    } else if (strcmp(posName, "e3") == 0) {
        return getBitboardFromSquare(20);
    } else if (strcmp(posName, "f3") == 0) {
        return getBitboardFromSquare(21);
    } else if (strcmp(posName, "g3") == 0) {
        return getBitboardFromSquare(22);
    } else if (strcmp(posName, "h3") == 0) {
        return getBitboardFromSquare(23);
    }
    
    if (strcmp(posName, "a4") == 0) {
        return getBitboardFromSquare(24);
    } else if (strcmp(posName, "b4") == 0) {
        return getBitboardFromSquare(25);
    } else if (strcmp(posName, "c4") == 0) {
        return getBitboardFromSquare(26);
    } else if (strcmp(posName, "d4") == 0) {
        return getBitboardFromSquare(27);
    } else if (strcmp(posName, "e4") == 0) {
        return getBitboardFromSquare(28);
    } else if (strcmp(posName, "f4") == 0) {
        return getBitboardFromSquare(29);
    } else if (strcmp(posName, "g4") == 0) {
        return getBitboardFromSquare(30);
    } else if (strcmp(posName, "h4") == 0) {
        return getBitboardFromSquare(31);
    }
    
    if (strcmp(posName, "a5") == 0) {
        return getBitboardFromSquare(32);
    } else if (strcmp(posName, "b5") == 0) {
        return getBitboardFromSquare(33);
    } else if (strcmp(posName, "c5") == 0) {
        return getBitboardFromSquare(34);
    } else if (strcmp(posName, "d5") == 0) {
        return getBitboardFromSquare(35);
    } else if (strcmp(posName, "e5") == 0) {
        return getBitboardFromSquare(36);
    } else if (strcmp(posName, "f5") == 0) {
        return getBitboardFromSquare(37);
    } else if (strcmp(posName, "g5") == 0) {
        return getBitboardFromSquare(38);
    } else if (strcmp(posName, "h5") == 0) {
        return getBitboardFromSquare(39);
    }
    
    if (strcmp(posName, "a6") == 0) {
        return getBitboardFromSquare(40);
    } else if (strcmp(posName, "b6") == 0) {
        return getBitboardFromSquare(41);
    } else if (strcmp(posName, "c6") == 0) {
        return getBitboardFromSquare(42);
    } else if (strcmp(posName, "d6") == 0) {
        return getBitboardFromSquare(43);
    } else if (strcmp(posName, "e6") == 0) {
        return getBitboardFromSquare(44);
    } else if (strcmp(posName, "f6") == 0) {
        return getBitboardFromSquare(45);
    } else if (strcmp(posName, "g6") == 0) {
        return getBitboardFromSquare(46);
    } else if (strcmp(posName, "h6") == 0) {
        return getBitboardFromSquare(47);
    }
    
    if (strcmp(posName, "a7") == 0) {
        return getBitboardFromSquare(48);
    } else if (strcmp(posName, "b7") == 0) {
        return getBitboardFromSquare(49);
    } else if (strcmp(posName, "c7") == 0) {
        return getBitboardFromSquare(50);
    } else if (strcmp(posName, "d7") == 0) {
        return getBitboardFromSquare(51);
    } else if (strcmp(posName, "e7") == 0) {
        return getBitboardFromSquare(52);
    } else if (strcmp(posName, "f7") == 0) {
        return getBitboardFromSquare(53);
    } else if (strcmp(posName, "g7") == 0) {
        return getBitboardFromSquare(54);
    } else if (strcmp(posName, "h7") == 0) {
        return getBitboardFromSquare(55);
    }
    
    if (strcmp(posName, "a8") == 0) {
        return getBitboardFromSquare(56);
    } else if (strcmp(posName, "b8") == 0) {
        return getBitboardFromSquare(57);
    } else if (strcmp(posName, "c8") == 0) {
        return getBitboardFromSquare(58);
    } else if (strcmp(posName, "d8") == 0) {
        return getBitboardFromSquare(59);
    } else if (strcmp(posName, "e8") == 0) {
        return getBitboardFromSquare(60);
    } else if (strcmp(posName, "f8") == 0) {
        return getBitboardFromSquare(61);
    } else if (strcmp(posName, "g8") == 0) {
        return getBitboardFromSquare(62);
    } else if (strcmp(posName, "h8") == 0) {
        return getBitboardFromSquare(63);
    }
    
    return 0;
}

int divide(u8 depth, u8 color) {
    
    char pieceName[2][8] = { { ' ', (char) 0, 'N', 'B', 'R', 'Q', 'K', '\0'}, { ' ', (char) 0, 'n', 'b', 'r', 'q', 'k', '\0'}};
    
    Move move_list[MAX_MOVES];
    
    u64 total_nodes = 0;
    u8 moveType;
    u64 nodes;
    u8 sideToMove = color;
    
    u8 noOfMoves = genMoves(move_list, sideToMove);
    u8 count = 0;
    
    clock_t start, end;
    double cpu_time_used;
    double nps;

	Move move;
    for (u8 i = 0; i < noOfMoves; i++) {
       
        start = clock();
       
		move = move_list[i];
		
        make_move(move.move);
        
        moveType = move_type(move.move);
        nodes = 0;
        
        if (isSqAttacked(bitScanForward(pieceBB[sideToMove][KING]), sideToMove)) {
            
        } else {
            count++;
            if (sideToMove == WHITE) {
                nodes = perft(depth - 1, BLACK);
            } else {
                nodes = perft(depth - 1, WHITE);
            }
            
            end  = clock();
            
            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            nps = (double)(nodes / (cpu_time_used * 1000000));
            
            total_nodes = total_nodes + nodes;
            
            printf("%d)%c%s-%s, ", count, pieceName[colorType(move.move)][pieceType(move.move)],
                   algebricPos(from_sq(move.move)), algebricPos(to_sq(move.move)));
            
            printf("%llu, Move type  - %d, castle flags -" BYTE_TO_BINARY_PATTERN", nps  - %7.3f MN/s\n", nodes, moveType, BYTE_TO_BINARY(moveStack[ply].castleFlags),  nps);
        }
        
        unmake_move(move.move);
    }
    
    printf("Total nodes -> %llu\n", total_nodes);
    return 0;
}

void checkUp() {
    
    if(timeSet && getTimeMs() > stopTime) {
        
        stopped = true;
    }
    
    readInput();
}

void initHashKey() {
	
	u64 bitboard;
	
	hashKey = 0ULL;
    for(u8 i = 0; i < 2; i++) {
    
        for(u8 j = PAWNS; j <= KING; j++) {
            
            bitboard = pieceBB[i][j];
            while (bitboard) {
                
				hashKey ^= zobrist[i][j][bitScanForward(bitboard)];
                bitboard &= bitboard - 1;
            }
        }
    }
	
	hashKey ^= KEY_FLAG_WHITE_CASTLE_QUEEN_SIDE ^ KEY_FLAG_WHITE_CASTLE_KING_SIDE;
	hashKey ^= KEY_FLAG_BLACK_CASTLE_QUEEN_SIDE ^ KEY_FLAG_BLACK_CASTLE_KING_SIDE;
}

void clearHashTable() {
	
	for(int i = 0; i < HASH_TABLE_SIZE; i++) {
		
		hashTable[i].key = 0ULL;
		hashTable[i].move = 0UL;
		hashTable[i].depth = -1;
		hashTable[i].value = -1;
		hashTable[i].flags = -1;
	}
}

void clearRepetitionHashTable() {
	
	for(int i = 0; i < 2048; i++) {
		
		repHashTable[i] = 0ULL;
	}
	
	repIndex = -1;
	fiftyMoves = 0;
}

void clearKillerMovesTable() {
	
	for(int i = 0; i < MAX_PLY; i++) {
		killerMoves[i].killerMove1 = 0UL;
		killerMoves[i].killerMove2 = 0UL;
	}
}

void clearHistoryTable() {
	
	for(u8 i = 0; i < 2; i++) {
		for(u8 j = 0; j < 8; j++) {
			for(u8 k = 0; k < 64; k++) {
	
				historyScore[i][j][k] = 0ULL;
			}
		}
	}
}

















