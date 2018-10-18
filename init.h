#ifndef __INIT_H__
#define __INIT_H__

void init_castling_rights() {
	castling_rights[0] = 1;
	castling_rights[1] = 1;
}

void init_piece_bb() {

	/* default positions of pieces on board */

	piece_bb[0][KING] = 0x0000000000000010U;
	piece_bb[0][QUEEN] = 0x0000000000000008U;
	piece_bb[0][BISHOPS] = 0x0000000000000024U;
	piece_bb[0][KNIGHTS] = 0x0000000000000042U;
	piece_bb[0][ROOKS] = 0x0000000000000081U;
	piece_bb[0][PAWNS] = 0x000000000000FF00U;
	piece_bb[0][PIECES] = 0x000000000000FFFFU;

	piece_bb[1][KING] = 0x1000000000000000U;
	piece_bb[1][QUEEN] = 0x0800000000000000U;
	piece_bb[1][BISHOPS] = 0x2400000000000000U;
	piece_bb[1][KNIGHTS] = 0x4200000000000000U;
	piece_bb[1][ROOKS] = 0x8100000000000000U;
	piece_bb[1][PAWNS] = 0x00FF000000000000U;
	piece_bb[1][PIECES] = 0xFFFF000000000000U;

	occupied = 0xFFFF00000000FFFFU;
	empty = 0x0000FFFFFFFF0000U;
}

void init_index_bb() {

	index_bb[0] = 0x0000000000000001U;
	index_bb[1] = 0x0000000000000002U;
	index_bb[2] = 0x0000000000000004U;
	index_bb[3] = 0x0000000000000008U;
	index_bb[4] = 0x0000000000000010U;
	index_bb[5] = 0x0000000000000020U;
	index_bb[6] = 0x0000000000000040U;
	index_bb[7] = 0x0000000000000080U;
	index_bb[8] = 0x0000000000000100U;
	index_bb[9] = 0x0000000000000200U;
	index_bb[10] = 0x0000000000000400U;
	index_bb[11] = 0x0000000000000800U;
	index_bb[12] = 0x0000000000001000U;
	index_bb[13] = 0x0000000000002000U;
	index_bb[14] = 0x0000000000004000U;
	index_bb[15] = 0x0000000000008000U;
	index_bb[16] = 0x0000000000010000U;
	index_bb[17] = 0x0000000000020000U;
	index_bb[18] = 0x0000000000040000U;
	index_bb[19] = 0x0000000000080000U;
	index_bb[20] = 0x0000000000100000U;
	index_bb[21] = 0x0000000000200000U;
	index_bb[22] = 0x0000000000400000U;
	index_bb[23] = 0x0000000000800000U;
	index_bb[24] = 0x0000000001000000U;
	index_bb[25] = 0x0000000002000000U;
	index_bb[26] = 0x0000000004000000U;
	index_bb[27] = 0x0000000008000000U;
	index_bb[28] = 0x0000000010000000U;
	index_bb[29] = 0x0000000020000000U;
	index_bb[30] = 0x0000000040000000U;
	index_bb[31] = 0x0000000080000000U;
	index_bb[32] = 0x0000000100000000U;
	index_bb[33] = 0x0000000200000000U;
	index_bb[34] = 0x0000000400000000U;
	index_bb[35] = 0x0000000800000000U;
	index_bb[36] = 0x0000001000000000U;
	index_bb[37] = 0x0000002000000000U;
	index_bb[38] = 0x0000004000000000U;
	index_bb[39] = 0x0000008000000000U;
	index_bb[40] = 0x0000010000000000U;
	index_bb[41] = 0x0000020000000000U;
	index_bb[42] = 0x0000040000000000U;
	index_bb[43] = 0x0000080000000000U;
	index_bb[44] = 0x0000100000000000U;
	index_bb[45] = 0x0000200000000000U;
	index_bb[46] = 0x0000400000000000U;
	index_bb[47] = 0x0000800000000000U;
	index_bb[48] = 0x0001000000000000U;
	index_bb[49] = 0x0002000000000000U;
	index_bb[50] = 0x0004000000000000U;
	index_bb[51] = 0x0008000000000000U;
	index_bb[52] = 0x0010000000000000U;
	index_bb[53] = 0x0020000000000000U;
	index_bb[54] = 0x0040000000000000U;
	index_bb[55] = 0x0080000000000000U;
	index_bb[56] = 0x0100000000000000U;
	index_bb[57] = 0x0200000000000000U;
	index_bb[58] = 0x0400000000000000U;
	index_bb[59] = 0x0800000000000000U;
	index_bb[60] = 0x1000000000000000U;
	index_bb[61] = 0x2000000000000000U;
	index_bb[62] = 0x4000000000000000U;
	index_bb[63] = 0x8000000000000000U;
}

#endif