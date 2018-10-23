/*
 * movegen.h
 *
 * Created on : May 3, 2017
 *     Author : Amar Thapa       
 */

#ifndef __MOVE_GEN_H__
#define __MOVE_GEN_H__

/* encode the move in a 32 bit integer */

u32 create_move(u8 promotion_type, u8 castle_dir, u8 move_type, u8 color, u8 c_piece, u8 piece, u8 from, u8 to);

u64 genMoves(u32 *move_list, u8 color);

u64 gen_pushes(u32 *move_list, u8 pos, u8 color);
u64 gen_attacks(u32 *move_list, u8 pos, u8 color);
u64 gen_special_moves(u32 *move_list, u8 pos, u8 color);

u64 gen_king_pushes(u32 *move_list, u8 pos, u8 color);
u64 gen_queen_pushes(u32 *move_list, u8 pos, u8 color);
u64 gen_bishop_pushes(u32 *move_list, u8 pos, u8 color);
u64 gen_knight_pushes(u32 *move_list, u8 pos, u8 color);
u64 gen_rook_pushes(u32 *move_list, u8 pos, u8 color);
u64 gen_pawn_pushes(u32 *move_list, u8 pos, u8 color);

u64 gen_king_attacks(u32 *move_list, u8 pos, u8 color);
u64 gen_queen_attacks(u32 *move_list, u8 pos, u8 color);
u64 gen_bishop_attacks(u32 *move_list, u8 pos, u8 color);
u64 gen_knight_attacks(u32 *move_list, u8 pos, u8 color);
u64 gen_rook_attacks(u32 *move_list, u8 pos, u8 color);
u64 gen_pawn_attacks(u32 *move_list, u8 pos, u8 color);

u64 gen_double_pushes(u32 *move_list, u8 pos, u8 color);
u64 gen_castling_moves(u32 *move_list, u8 pos, u8 color);
u64 gen_enpassant_moves(u32 *move_list, u8 pos, u8 color);
u64 gen_promotions(u32 *move_list, u8 pos, u8 color);

#endif






