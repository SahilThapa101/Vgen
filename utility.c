#include <stdlib.h>
#include <stdio.h>

#include "utility.h"
#include "nonslidingmoves.h"
#include "magicmoves.h"

void print_bb(u64 board) {

	char *output = malloc(65 * sizeof(char));
	printf(" ---------- ");
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

	char *output = malloc(65 * sizeof(char));
	printf(" ---------- ");
	int row;
	int col;
	for (row = 7; row >= 0; row--) {
		printf("\n|%d ", row + 1);
		for (col = 0; col < 8; col++) {
			if ((((u64) 1) << ((row * 8 + col)) & board)
					== (u64) 1 << (row * 8 + col)) {

				if ((u64) 1 << (row * 8 + col) & piece_bb[WHITE][KING])
					putchar('K');
				if ((u64) 1 << (row * 8 + col) & piece_bb[WHITE][QUEEN])
					putchar('Q');
				if ((u64) 1 << (row * 8 + col) & piece_bb[WHITE][BISHOPS])
					putchar('B');
				if ((u64) 1 << (row * 8 + col) & piece_bb[WHITE][KNIGHTS])
					putchar('N');
				if ((u64) 1 << (row * 8 + col) & piece_bb[WHITE][ROOKS])
					putchar('R');
				if ((u64) 1 << (row * 8 + col) & piece_bb[WHITE][PAWNS])
					putchar('P');

				if ((u64) 1 << (row * 8 + col) & piece_bb[BLACK][KING])
					putchar('k');
				if ((u64) 1 << (row * 8 + col) & piece_bb[BLACK][QUEEN])
					putchar('q');
				if ((u64) 1 << (row * 8 + col) & piece_bb[BLACK][BISHOPS])
					putchar('b');
				if ((u64) 1 << (row * 8 + col) & piece_bb[BLACK][KNIGHTS])
					putchar('n');
				if ((u64) 1 << (row * 8 + col) & piece_bb[BLACK][ROOKS])
					putchar('r');
				if ((u64) 1 << (row * 8 + col) & piece_bb[BLACK][PAWNS])
					putchar('p');
			} else
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

const int index64[64] = { 0, 47, 1, 56, 48, 27, 2, 60, 57, 49, 41, 37, 28, 16,
		3, 61, 54, 58, 35, 52, 50, 42, 21, 44, 38, 32, 29, 23, 17, 11, 4, 62,
		46, 55, 26, 59, 40, 36, 15, 53, 34, 51, 20, 43, 31, 22, 10, 45, 25, 39,
		14, 33, 19, 30, 9, 24, 13, 18, 8, 12, 7, 6, 5, 63 };

int bit_scan_forward(u64 board) {
	const u64 debruijn64 = 285870213051386505U;

	return index64[((board ^ (board - 1)) * debruijn64) >> 58];
}

/* function to check if a square is attacked */

int is_sq_attacked(u8 sq, u8 color) {

	u64 attacks;

	/* check if a king is attacking a sq */

	attacks = get_king_attacks(sq);

	if (attacks & piece_bb[color ^ 1][KING])
		return 1;

	/* check if a queen is attacking a sq */

	attacks = Qmagic(sq, occupied);

	if (attacks & piece_bb[color ^ 1][QUEEN])
		return 1;

	/* check if a bishop is attacking a square */

	attacks = Bmagic(sq, occupied);

	if (attacks & piece_bb[color ^ 1][BISHOPS])
		return 1;

	/* check if a knight is attacking a square */

	attacks = get_knight_attacks(sq);

	if (attacks & piece_bb[color ^ 1][KNIGHTS])
		return 1;

	/* check if a rook is attacking a square */

	attacks = Rmagic(sq, occupied);

	if (attacks & piece_bb[color ^ 1][ROOKS])
		return 1;

	/* check if a pawn is attacking a square */

	if (color == WHITE)
		attacks = ((index_bb[sq] << 7) & NOT_H_FILE)
				| ((index_bb[sq] << 9) & NOT_A_FILE);
	else
		attacks = ((index_bb[sq] >> 7) & NOT_A_FILE)
				| ((index_bb[sq] >> 9) & NOT_H_FILE);

	if (attacks & piece_bb[color ^ 1][PAWNS])
		return 1;

	return 0;
}

u64 returnBB(int sq) {
	u64 bb;
	switch (sq) {
	case 0:
		bb = 0x0000000000000001U;
		break;
	case 1:
		bb = 0x0000000000000002U;
		break;
	case 2:
		bb = 0x0000000000000004U;
		break;
	case 3:
		bb = 0x0000000000000008U;
		break;
	case 4:
		bb = 0x0000000000000010U;
		break;
	case 5:
		bb = 0x0000000000000020U;
		break;
	case 6:
		bb = 0x0000000000000040U;
		break;
	case 7:
		bb = 0x0000000000000080U;
		break;
	case 8:
		bb = 0x0000000000000100U;
		break;
	case 9:
		bb = 0x0000000000000200U;
		break;
	case 10:
		bb = 0x0000000000000400U;
		break;
	case 11:
		bb = 0x0000000000000800U;
		break;
	case 12:
		bb = 0x0000000000001000U;
		break;
	case 13:
		bb = 0x0000000000002000U;
		break;
	case 14:
		bb = 0x0000000000004000U;
		break;
	case 15:
		bb = 0x0000000000008000U;
		break;
	case 16:
		bb = 0x0000000000010000U;
		break;
	case 17:
		bb = 0x0000000000020000U;
		break;
	case 18:
		bb = 0x0000000000040000U;
		break;
	case 19:
		bb = 0x0000000000080000U;
		break;
	case 20:
		bb = 0x0000000000100000U;
		break;
	case 21:
		bb = 0x0000000000200000U;
		break;
	case 22:
		bb = 0x0000000000400000U;
		break;
	case 23:
		bb = 0x0000000000800000U;
		break;
	case 24:
		bb = 0x0000000001000000U;
		break;
	case 25:
		bb = 0x0000000002000000U;
		break;
	case 26:
		bb = 0x0000000004000000U;
		break;
	case 27:
		bb = 0x0000000008000000U;
		break;
	case 28:
		bb = 0x0000000010000000U;
		break;
	case 29:
		bb = 0x0000000020000000U;
		break;
	case 30:
		bb = 0x0000000040000000U;
		break;
	case 31:
		bb = 0x0000000080000000U;
		break;
	case 32:
		bb = 0x0000000100000000U;
		break;
	case 33:
		bb = 0x0000000200000000U;
		break;
	case 34:
		bb = 0x0000000400000000U;
		break;
	case 35:
		bb = 0x0000000800000000U;
		break;
	case 36:
		bb = 0x0000001000000000U;
		break;
	case 37:
		bb = 0x0000002000000000U;
		break;
	case 38:
		bb = 0x0000004000000000U;
		break;
	case 39:
		bb = 0x0000008000000000U;
		break;
	case 40:
		bb = 0x0000010000000000U;
		break;
	case 41:
		bb = 0x0000020000000000U;
		break;
	case 42:
		bb = 0x0000040000000000U;
		break;
	case 43:
		bb = 0x0000080000000000U;
		break;
	case 44:
		bb = 0x0000100000000000U;
		break;
	case 45:
		bb = 0x0000200000000000U;
		break;
	case 46:
		bb = 0x0000400000000000U;
		break;
	case 47:
		bb = 0x0000800000000000U;
		break;
	case 48:
		bb = 0x0001000000000000U;
		break;
	case 49:
		bb = 0x0002000000000000U;
		break;
	case 50:
		bb = 0x0004000000000000U;
		break;
	case 51:
		bb = 0x0008000000000000U;
		break;
	case 52:
		bb = 0x0010000000000000U;
		break;
	case 53:
		bb = 0x0020000000000000U;
		break;
	case 54:
		bb = 0x0040000000000000U;
		break;
	case 55:
		bb = 0x0080000000000000U;
		break;
	case 56:
		bb = 0x0100000000000000U;
		break;
	case 57:
		bb = 0x0200000000000000U;
		break;
	case 58:
		bb = 0x0400000000000000U;
		break;
	case 59:
		bb = 0x0800000000000000U;
		break;
	case 60:
		bb = 0x1000000000000000U;
		break;
	case 61:
		bb = 0x2000000000000000U;
		break;
	case 62:
		bb = 0x4000000000000000U;
		break;
	case 63:
		bb = 0x8000000000000000U;
		break;
	default:
		break;
	}
	return bb;
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
		return returnBB(0);
	} else if (strcmp(posName, "b1") == 0) {
		return returnBB(1);
	} else if (strcmp(posName, "c1") == 0) {
		return returnBB(2);
	} else if (strcmp(posName, "d1") == 0) {
		return returnBB(3);
	} else if (strcmp(posName, "e1") == 0) {
		return returnBB(4);
	} else if (strcmp(posName, "f1") == 0) {
		return returnBB(5);
	} else if (strcmp(posName, "g1") == 0) {
		return returnBB(6);
	} else if (strcmp(posName, "h1") == 0) {
		return returnBB(7);
	}

	if (strcmp(posName, "a2") == 0) {
		return returnBB(8);
	} else if (strcmp(posName, "b2") == 0) {
		return returnBB(9);
	} else if (strcmp(posName, "c2") == 0) {
		return returnBB(10);
	} else if (strcmp(posName, "d2") == 0) {
		return returnBB(11);
	} else if (strcmp(posName, "e2") == 0) {
		return returnBB(12);
	} else if (strcmp(posName, "f2") == 0) {
		return returnBB(13);
	} else if (strcmp(posName, "g2") == 0) {
		return returnBB(14);
	} else if (strcmp(posName, "h2") == 0) {
		return returnBB(15);
	}

	if (strcmp(posName, "a3") == 0) {
		return returnBB(16);
	} else if (strcmp(posName, "b3") == 0) {
		return returnBB(17);
	} else if (strcmp(posName, "c3") == 0) {
		return returnBB(18);
	} else if (strcmp(posName, "d3") == 0) {
		return returnBB(19);
	} else if (strcmp(posName, "e3") == 0) {
		return returnBB(20);
	} else if (strcmp(posName, "f3") == 0) {
		return returnBB(21);
	} else if (strcmp(posName, "g3") == 0) {
		return returnBB(22);
	} else if (strcmp(posName, "h3") == 0) {
		return returnBB(23);
	}

	if (strcmp(posName, "a4") == 0) {
		return returnBB(24);
	} else if (strcmp(posName, "b4") == 0) {
		return returnBB(25);
	} else if (strcmp(posName, "c4") == 0) {
		return returnBB(26);
	} else if (strcmp(posName, "d4") == 0) {
		return returnBB(27);
	} else if (strcmp(posName, "e4") == 0) {
		return returnBB(28);
	} else if (strcmp(posName, "f4") == 0) {
		return returnBB(29);
	} else if (strcmp(posName, "g4") == 0) {
		return returnBB(30);
	} else if (strcmp(posName, "h4") == 0) {
		return returnBB(31);
	}

	if (strcmp(posName, "a5") == 0) {
		return returnBB(32);
	} else if (strcmp(posName, "b5") == 0) {
		return returnBB(33);
	} else if (strcmp(posName, "c5") == 0) {
		return returnBB(34);
	} else if (strcmp(posName, "d5") == 0) {
		return returnBB(35);
	} else if (strcmp(posName, "e5") == 0) {
		return returnBB(36);
	} else if (strcmp(posName, "f5") == 0) {
		return returnBB(37);
	} else if (strcmp(posName, "g5") == 0) {
		return returnBB(38);
	} else if (strcmp(posName, "h5") == 0) {
		return returnBB(39);
	}

	if (strcmp(posName, "a6") == 0) {
		return returnBB(40);
	} else if (strcmp(posName, "b6") == 0) {
		return returnBB(41);
	} else if (strcmp(posName, "c6") == 0) {
		return returnBB(42);
	} else if (strcmp(posName, "d6") == 0) {
		return returnBB(43);
	} else if (strcmp(posName, "e6") == 0) {
		return returnBB(44);
	} else if (strcmp(posName, "f6") == 0) {
		return returnBB(45);
	} else if (strcmp(posName, "g6") == 0) {
		return returnBB(46);
	} else if (strcmp(posName, "h6") == 0) {
		return returnBB(47);
	}

	if (strcmp(posName, "a7") == 0) {
		return returnBB(48);
	} else if (strcmp(posName, "b7") == 0) {
		return returnBB(49);
	} else if (strcmp(posName, "c7") == 0) {
		return returnBB(50);
	} else if (strcmp(posName, "d7") == 0) {
		return returnBB(51);
	} else if (strcmp(posName, "e7") == 0) {
		return returnBB(52);
	} else if (strcmp(posName, "f7") == 0) {
		return returnBB(53);
	} else if (strcmp(posName, "g7") == 0) {
		return returnBB(54);
	} else if (strcmp(posName, "h7") == 0) {
		return returnBB(55);
	}

	if (strcmp(posName, "a8") == 0) {
		return returnBB(56);
	} else if (strcmp(posName, "b8") == 0) {
		return returnBB(57);
	} else if (strcmp(posName, "c8") == 0) {
		return returnBB(58);
	} else if (strcmp(posName, "d8") == 0) {
		return returnBB(59);
	} else if (strcmp(posName, "e8") == 0) {
		return returnBB(60);
	} else if (strcmp(posName, "f8") == 0) {
		return returnBB(61);
	} else if (strcmp(posName, "g8") == 0) {
		return returnBB(62);
	} else if (strcmp(posName, "h8") == 0) {
		return returnBB(63);
	}

}

char pieceName[2][7] = { { 'K', 'Q', 'B', 'N', 'R', (char) 0, (char) 0 }, { 'k',
		'q', 'b', 'n', 'r', (char) 0, (char) 0 } };

int divide(u8 depth, u8 color) {
	u32 move_list[MAX_MOVES];

	u64 total_nodes = 0;
	u64 piece;
	u64 nodes;
	for (u8 i = 0; i < gen_moves(move_list, color); i++) {

		//ply=ply+1;

		make_move(move_list[i]);
		piece = piece_type(move_list[i]);
		nodes = 0;

		if (!is_sq_attacked(bit_scan_forward(piece_bb[color][KING]), color)) {
			printf("%d  %c%s-%s -> ", i + 1, pieceName[color][piece],
					algebricPos(from_sq(move_list[i])),
					algebricPos(to_sq(move_list[i])));

			nodes = perft(depth - 1, color ^ 1);

			total_nodes = total_nodes + nodes;
			printf("%llu\n", nodes);
		}

		unmake_move(move_list[i]);
		epFlag = 0;

		//ply = ply - 1;
	}

	printf("Total nodes -> %llu\n", total_nodes);
	return 0;
}

