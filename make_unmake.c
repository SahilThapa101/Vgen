#include <stdbool.h>
#include "make_unmake.h"
#include "utility.h"
#include "Globals.h"


void make_move(u32 move) {

	u64 from_bb, to_bb, from_to_bb, piece, c_piece;

	from_bb = index_bb[from_sq(move)];
	to_bb = index_bb[to_sq(move)];

	from_to_bb = from_bb ^ to_bb;

	piece = piece_type(move);
	c_piece = c_piece_type(move);
	color = color_type(move);

	switch(move_type(move)) {
	
		case 0 : 
					quiet++;
					
					piece_bb[color][piece] ^= from_to_bb;
					piece_bb[color][PIECES] ^= from_to_bb;
					occupied ^= from_to_bb;
					empty ^= from_to_bb;

					/*switch(color) {
						case 0: 
								if(castling_rights[0]) {
									switch(piece) {

										case KING:	hist[ply].castle_flags &= 0x1100;
													castling_rights[0] = 0;

													break;

										case ROOKS: 
													if(from_bb == 0)
														hist[ply].castle_flags &= 0x1110;
													else
														hist[ply].castle_flags &= 0x1101;

													break;
									}
								}	

								break;

						case 1: 
								if(castling_rights[1]) {
									switch(piece) {

										case KING:	hist[ply].castle_flags &= 0x0011;
													castling_rights[1] = 0;

													break;

										case ROOKS: 
													if(from_bb == 0x0100000000000000)
														hist[ply].castle_flags &= 0x1011;
													else
														hist[ply].castle_flags &= 0x0111;

													break;
									}
								}
								
								break;
					}*/
					
					break;
	
		case 1 :
					cap++;
					piece_bb[color][piece] ^= from_to_bb;
					piece_bb[color][PIECES] ^= from_to_bb;
					piece_bb[color ^ 1][c_piece] ^= to_bb;
					piece_bb[color ^ 1][PIECES] ^= to_bb;

					occupied ^= from_bb;
					empty ^= from_bb;

					// why this castling stuff here
/*
					switch(color) {
						case 0: 
								if(c_piece == ROOKS) {
									if(to_bb == 0U)
										hist[ply].castle_flags &= 0x1110;
									else
										hist[ply].castle_flags &= 0x1101;
								}

								break;

						case 1: 
								if(c_piece == ROOKS) {
									if(to_bb == 0x0100000000000000U)
										hist[ply].castle_flags &= 0x1011;
									else
										hist[ply].castle_flags &= 0x0111;
								}

								break;
					}
*/
					break;
	
		case 2 : 
					quiet++;
					
					hist[ply].ep_sq = (from_bb << 8) >> 16 * color;
					hist[ply].ep_flag = 1;

					piece_bb[color][piece] ^= from_to_bb;
					piece_bb[color][PIECES] ^= from_to_bb;

					occupied ^= from_to_bb;
					empty ^= from_to_bb;

					break;

		case 3 :	
					en++;

					piece_bb[color][piece] ^= from_to_bb;
					piece_bb[color][PIECES] ^= from_to_bb;

					piece_bb[color ^ 1][c_piece] ^= (to_bb << 8) >> 16 * (color ^ 1);
					piece_bb[color ^ 1][PIECES] ^= (to_bb << 8) >> 16 * (color ^ 1);

					occupied ^= (from_to_bb | (to_bb >> 8) << 16 * (color ^ 1));
					empty ^= (from_to_bb | (to_bb >> 8) << 16 * (color ^ 1));				

					break;
		
		case 4 : 
					cas++;	

					switch(castle_dir(move)) {
						case 0: 
								if(!(is_sq_attacked(1, color) & is_sq_attacked(2, color) & is_sq_attacked(3, color))) {
								
									hist[ply].castle_flags &= 0x1110;
									castling_rights[0] = 0;

									piece_bb[color][piece] ^= from_to_bb;
									
									piece_bb[color][c_piece] ^= (0x0000 ^ 0x0008);

									piece_bb[color][PIECES] &= ~0x0000000000000011U;
									piece_bb[color][PIECES] |= 0x000000000000000CU;

									occupied = piece_bb[color][PIECES] | piece_bb[color ^ 1][PIECES];
									empty = ~occupied;
									
								}

								break;

						case 1:
								if(!(is_sq_attacked(5, color) & is_sq_attacked(6, color))) {
								
									hist[ply].castle_flags &= 0x1101;
									castling_rights[0] = 0;

									piece_bb[color][piece] ^= from_to_bb;
									
									piece_bb[color][c_piece] ^= (0x0080U ^ 0x0020U);

									piece_bb[color][PIECES] &= ~0x0000000000000090U;
									piece_bb[color][PIECES] |=  0x0000000000000060U;

									occupied = piece_bb[color][PIECES] | piece_bb[color ^ 1][PIECES];
									empty = ~occupied;
									
								}

								break;

						case 2: 
								if(!(is_sq_attacked(57, color) & is_sq_attacked(58, color) & is_sq_attacked(59, color))) {
								
									hist[ply].castle_flags &= 0x1011;
									castling_rights[1] = 0;

									piece_bb[color][piece] ^= from_to_bb;
									
									piece_bb[color][c_piece] ^= (0x0100000000000000U ^ 0x0800000000000000U);

									piece_bb[color][PIECES] &= ~0x1100000000000000U;
									piece_bb[color][PIECES] |=  0x0C00000000000000U;

									occupied = piece_bb[color][PIECES] | piece_bb[color ^ 1][PIECES];
									empty = ~occupied;
									
								}

								break;

						case 3: 
								if(!(is_sq_attacked(61, color) & is_sq_attacked(62, color))) {
								
									hist[ply].castle_flags &= 0x0111;
									castling_rights[1] = 0;

									piece_bb[color][piece] ^= from_to_bb;
									
									piece_bb[color][c_piece] ^= (0x8000000000000000U ^ 0x2000000000000000U);

									piece_bb[color][PIECES] &= ~0x9000000000000000U;
									piece_bb[color][PIECES] |=  0x6000000000000000U;

									occupied = piece_bb[color][PIECES] | piece_bb[color ^ 1][PIECES];
									empty = ~occupied;
									
								}

								break;
					}

					break;
	
		case 5 : 
				switch(prom_type(move)) {
					case 0: 
							piece_bb[color][PAWNS] ^= from_bb;
							piece_bb[color][QUEEN] ^= to_bb;
							piece_bb[color][PIECES] ^= from_to_bb;

							if(c_piece != DUMMY) {
								piece_bb[color ^ 1][c_piece] ^= to_bb;
								piece_bb[color ^ 1][PIECES] ^= to_bb; 

								occupied ^= from_bb;
								empty ^= from_bb;

							} else {
								occupied ^= from_to_bb;
								empty ^= from_to_bb;
							}

							break;
					case 1: 
							piece_bb[color][PAWNS] ^= from_bb;
							piece_bb[color][ROOKS] ^= to_bb;
							piece_bb[color][PIECES] ^= from_to_bb;

							if(c_piece != DUMMY) {
								piece_bb[color ^ 1][c_piece] ^= to_bb;
								piece_bb[color ^ 1][PIECES] ^= to_bb; 

								occupied ^= from_bb;
								empty ^= from_bb;

							} else {
								occupied ^= from_to_bb;
								empty ^= from_to_bb;
							}

							break;
					case 2: 
							piece_bb[color][PAWNS] ^= from_bb;
							piece_bb[color][KNIGHTS] ^= to_bb;
							piece_bb[color][PIECES] ^= from_to_bb;

							if(c_piece != DUMMY) {
								piece_bb[color ^ 1][c_piece] ^= to_bb;
								piece_bb[color ^ 1][PIECES] ^= to_bb; 

								occupied ^= from_bb;
								empty ^= from_bb;

							} else {
								occupied ^= from_to_bb;
								empty ^= from_to_bb;
							}

							break;
					case 3:
							piece_bb[color][PAWNS] ^= from_bb;
							piece_bb[color][BISHOPS] ^= to_bb;
							piece_bb[color][PIECES] ^= from_to_bb;

							if(c_piece != DUMMY) {
								piece_bb[color ^ 1][c_piece] ^= to_bb;
								piece_bb[color ^ 1][PIECES] ^= to_bb; 

								occupied ^= from_bb;
								empty ^= from_bb;

							} else {
								occupied ^= from_to_bb;
								empty ^= from_to_bb;
							}
							break;
				}
				break;
		default:
			break;
	}


}

void unmake_move(u32 move) {
	u64 from_bb, to_bb, from_to_bb, piece, c_piece, color;

	from_bb = index_bb[from_sq(move)];
	to_bb = index_bb[to_sq(move)];

	from_to_bb = from_bb ^ to_bb;

	piece = piece_type(move);
	c_piece = c_piece_type(move);
	color = color_type(move);

	switch(move_type(move)) {
		case 0 :	
					piece_bb[color][piece] ^= from_to_bb;
					piece_bb[color][PIECES] ^= from_to_bb;
					
					occupied ^= from_to_bb;
					empty	^= from_to_bb;

					break;
		case 1 :
					piece_bb[color][piece] ^= from_to_bb;
					piece_bb[color][PIECES] ^= from_to_bb;

					piece_bb[color ^ 1][c_piece] ^= to_bb;
					piece_bb[color ^ 1][PIECES] ^= to_bb;

					occupied ^= from_bb;
					empty ^= from_bb;

					break;
		case 2 : 
					piece_bb[color][piece] ^= from_to_bb;
					piece_bb[color][PIECES] ^= from_to_bb;
					
					occupied ^= from_to_bb;
					empty	^= from_to_bb;

					break;
		case 3 : 
					piece_bb[color][piece] ^= from_to_bb;
					piece_bb[color][PIECES] ^= from_to_bb;

					piece_bb[color ^ 1][c_piece] ^= (to_bb << 8) >> 16 * (color ^ 1);
					piece_bb[color ^ 1][PIECES] ^= (to_bb << 8) >> 16 * (color ^ 1);

					occupied ^= (from_to_bb | (to_bb >> 8) << 16 * (color ^ 1));
					empty ^= (from_to_bb | (to_bb >> 8) << 16 * (color ^ 1));				

					break;
		case 4 :	
					switch(castle_dir(move)) {
						case 0: 
								castling_rights[0] = 1;

								piece_bb[color][piece] ^= from_to_bb;
								
								piece_bb[color][c_piece] ^= (0x0000 ^ 0x0008);

								piece_bb[color][PIECES] &= 0x000000000000000CU;
								piece_bb[color][PIECES] |= ~0x0000000000000011U;

								occupied = piece_bb[color][PIECES] | piece_bb[color ^ 1][PIECES];
								empty = ~occupied;
							
								break;

						case 1:
								castling_rights[0] = 1;

								piece_bb[color][piece] ^= from_to_bb;
								
								piece_bb[color][c_piece] ^= (0x0080U ^ 0x0020U);

								piece_bb[color][PIECES] &=  0x0000000000000060U;
								piece_bb[color][PIECES] |= ~0x0000000000000090U;

								occupied = piece_bb[color][PIECES] | piece_bb[color ^ 1][PIECES];
								empty = ~occupied;
								
							break;

						case 2:
								castling_rights[1] = 1;

								piece_bb[color][piece] ^= from_to_bb;
								
								piece_bb[color][c_piece] ^= (0x0100000000000000U ^ 0x0800000000000000U);

								piece_bb[color][PIECES] &=  0x0C00000000000000U;
								piece_bb[color][PIECES] |= ~0x1100000000000000U;

								occupied = piece_bb[color][PIECES] | piece_bb[color ^ 1][PIECES];
								empty = ~occupied;
								
						
								break;

						case 3: 
								castling_rights[1] = 1;

								piece_bb[color][piece] ^= from_to_bb;
								
								piece_bb[color][c_piece] ^= (0x8000000000000000U ^ 0x2000000000000000U);

								piece_bb[color][PIECES] &=  0x6000000000000000U;
								piece_bb[color][PIECES] |= ~0x9000000000000000U;

								occupied = piece_bb[color][PIECES] | piece_bb[color ^ 1][PIECES];
								empty = ~occupied;
				
								break;
					}

					break;
		case 5 : 
					switch(prom_type(move)) {
					case 0: 
							piece_bb[color][PAWNS] ^= from_bb;
							piece_bb[color][QUEEN] ^= to_bb;
							piece_bb[color][PIECES] ^= from_to_bb;

							if(c_piece != DUMMY) {
								piece_bb[color ^ 1][c_piece] ^= to_bb;
								piece_bb[color ^ 1][PIECES] ^= to_bb; 

								occupied ^= from_bb;
								empty ^= from_bb;

							} else {
								occupied ^= from_to_bb;
								empty ^= from_to_bb;
							}

							break;
					case 1: 
							piece_bb[color][PAWNS] ^= from_bb;
							piece_bb[color][ROOKS] ^= to_bb;
							piece_bb[color][PIECES] ^= from_to_bb;

							if(c_piece != DUMMY) {
								piece_bb[color ^ 1][c_piece] ^= to_bb;
								piece_bb[color ^ 1][PIECES] ^= to_bb; 

								occupied ^= from_bb;
								empty ^= from_bb;

							} else {
								occupied ^= from_to_bb;
								empty ^= from_to_bb;
							}

							break;
					case 2: 
							piece_bb[color][PAWNS] ^= from_bb;
							piece_bb[color][KNIGHTS] ^= to_bb;
							piece_bb[color][PIECES] ^= from_to_bb;

							if(c_piece != DUMMY) {
								piece_bb[color ^ 1][c_piece] ^= to_bb;
								piece_bb[color ^ 1][PIECES] ^= to_bb; 

								occupied ^= from_bb;
								empty ^= from_bb;

							} else {
								occupied ^= from_to_bb;
								empty ^= from_to_bb;
							}

							break;
					case 3:
							piece_bb[color][PAWNS] ^= from_bb;
							piece_bb[color][BISHOPS] ^= to_bb;
							piece_bb[color][PIECES] ^= from_to_bb;

							if(c_piece != DUMMY) {
								piece_bb[color ^ 1][c_piece] ^= to_bb;
								piece_bb[color ^ 1][PIECES] ^= to_bb; 

								occupied ^= from_bb;
								empty ^= from_bb;

							} else {
								occupied ^= from_to_bb;
								empty ^= from_to_bb;
							}

							break;
				}
				
				break;
	}
}
