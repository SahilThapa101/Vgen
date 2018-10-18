#include <stdlib.h>
#include <stdio.h>

#include "utility.h"
#include "nonslidingmoves.h"
#include "magicmoves.h"


void print_bb(u64 board) {

	char *output = malloc(65*sizeof(char));
	printf(" ---------- ");
	int row;
	int col;
	for (row=7;row>=0; row--) {
		printf("\n|%d ", row+1);
		for (col=0; col<8; col++) {
			if ((((u64)1)<<((row*8+col)) & board) == (u64)1<<(row*8+col)) {
				putchar('x');				
			}
			else
				putchar('.');
		}
		printf("|");
	}
	printf("\n|  abcdefgh|\n");
	printf(" ---------- \n");
}


void print_board(u64 board) {

	char *output = malloc(65*sizeof(char));
	printf(" ---------- ");
	int row;
	int col;
	for (row=7;row>=0; row--) {
		printf("\n|%d ", row+1);
		for (col=0; col<8; col++) {
			if ((((u64)1)<<((row*8+col)) & board) == (u64)1<<(row*8+col)) {
				
				if((u64)1<<(row*8+col) & piece_bb[WHITE][KING])
					putchar('K');
				if((u64)1<<(row*8+col) & piece_bb[WHITE][QUEEN])
					putchar('Q');
				if((u64)1<<(row*8+col) & piece_bb[WHITE][BISHOPS])
					putchar('B');
				if((u64)1<<(row*8+col) & piece_bb[WHITE][KNIGHTS])
					putchar('N');
				if((u64)1<<(row*8+col) & piece_bb[WHITE][ROOKS])
					putchar('R');
				if((u64)1<<(row*8+col) & piece_bb[WHITE][PAWNS])
					putchar('P');

				if((u64)1<<(row*8+col) & piece_bb[BLACK][KING])
					putchar('k');
				if((u64)1<<(row*8+col) & piece_bb[BLACK][QUEEN])
					putchar('q');
				if((u64)1<<(row*8+col) & piece_bb[BLACK][BISHOPS])
					putchar('b');
				if((u64)1<<(row*8+col) & piece_bb[BLACK][KNIGHTS])
					putchar('n');
				if((u64)1<<(row*8+col) & piece_bb[BLACK][ROOKS])
					putchar('r');
				if((u64)1<<(row*8+col) & piece_bb[BLACK][PAWNS])
					putchar('p');
			}
			else
				putchar('.');
		}
		printf("|");
	}
	printf("\n|  abcdefgh|\n");
	printf(" ---------- \n");
}


/**
 *  * bitScanForward
 *   * @author Kim Walisch (2012)
 *    * @param bb bitboard to scan
 *     * @precondition bb != 0
 *      * @return index (0..63) of least significant one bit
 *       */

const int index64[64] = {
	 0, 47,  1, 56, 48, 27,  2, 60,
	57, 49, 41, 37, 28, 16,  3, 61,
	54, 58, 35, 52, 50, 42, 21, 44,
	38, 32, 29, 23, 17, 11,  4, 62,
	46, 55, 26, 59, 40, 36, 15, 53,
	34, 51, 20, 43, 31, 22, 10, 45,
	25, 39, 14, 33, 19, 30,  9, 24,
	13, 18,  8, 12,  7,  6,  5, 63
};

int bit_scan_forward(u64 board) {
	const u64 debruijn64 = 285870213051386505U;
	
	return index64[((board ^ (board - 1)) * debruijn64) >> 58];
}



/* function to check if a square is attacked */

bool is_sq_attacked(u8 sq, u8 color) {

	u64 attacks;	
	
	/* check if a king is attacking a sq */

	attacks = get_king_attacks(sq);

	if(attacks & piece_bb[color ^ 1][KING])
		return true;


	/* check if a queen is attacking a sq */

	attacks = Qmagic(sq, occupied);

	if(attacks & piece_bb[color ^ 1][QUEEN])
		return true;

	/* check if a bishop is attacking a square */

	attacks = Bmagic(sq, occupied);

	if(attacks & piece_bb[color ^ 1][BISHOPS])
		return true;

	/* check if a knight is attacking a square */

	attacks = get_knight_attacks(sq);

	if(attacks & piece_bb[color ^ 1][KNIGHTS])
		return true;

	/* check if a rook is attacking a square */

	attacks = Rmagic(sq, occupied);

	if(attacks & piece_bb[color ^ 1][ROOKS])
		return true;

	/* check if a pawn is attacking a square */

	if (color == WHITE) 
		attacks = ((index_bb[sq] << 7) & NOT_H_FILE) | ((index_bb[sq] << 9)  & NOT_A_FILE);
	else 
		attacks = ((index_bb[sq] >> 7) & NOT_A_FILE) | ((index_bb[sq] >> 9) & NOT_H_FILE);
	

	if (attacks & piece_bb[color ^ 1][PAWNS])
		return true;

	return false;
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
}

int  divide(u8 depth, u8 color) {
	u32 move_list[MAX_MOVES];
	
	const u8 COLOR = color;
	u64 total_nodes = 0;
	for(u8 i = 0; i < gen_moves(move_list, COLOR); i++) {
		
		ply=ply+1;
		make_move(move_list[i]);
		
		u64 nodes = 0;
		if (!is_sq_attacked(bit_scan_forward(piece_bb[COLOR][KING]), COLOR)) {
			printf("%s%s -> ", algebricPos(from_sq(move_list[i])), algebricPos(to_sq(move_list[i])));
			
			nodes = perft(depth - 1, COLOR ^ 1);
			
			total_nodes = total_nodes + nodes;
			printf("%llu\n", nodes);
		} 

		unmake_move(move_list[i]);
		ply = ply - 1;
	} 

	printf("Total nodes -> %llu\n", total_nodes);
	return 0;
}


