/*
 * movegen.h
 *
 * Created on : May 3, 2017
 *     Author : Amar Thapa       
 */

#ifndef __MOVE_GEN_H__
#define __MOVE_GEN_H__

/* encode the move in a 32 bit integer */

u32 createMove(u8 promotion_type, u8 castle_dir, u8 move_type, u8 color, u8 c_piece, u8 piece, u8 from, u8 to);

u64 genCaptures(u32 *move_list, u8 color);

u64 genMoves(u32 *move_list, u8 color);

u64 genPushes(u32 *move_list, u8 pos, u8 color);
u64 genAttacks(u32 *move_list, u8 pos, u8 color);
u64 genSpecialMoves(u32 *move_list, u8 pos, u8 color);

u64 genKingPushes(u32 *move_list, u8 pos, u8 color);
u64 genQueenPushes(u32 *move_list, u8 pos, u8 color);
u64 genBishopPushes(u32 *move_list, u8 pos, u8 color);
u64 genKnightPushes(u32 *move_list, u8 pos, u8 color);
u64 genRookPushes(u32 *move_list, u8 pos, u8 color);
u64 genPawnPushes(u32 *move_list, u8 pos, u8 color);

u64 genKingAttacks(u32 *move_list, u8 pos, u8 color);
u64 genQueenAttacks(u32 *move_list, u8 pos, u8 color);
u64 genBishopAttacks(u32 *move_list, u8 pos, u8 color);
u64 genKnightAttacks(u32 *move_list, u8 pos, u8 color);
u64 genRookAttacks(u32 *move_list, u8 pos, u8 color);
u64 genPawnAttacks(u32 *move_list, u8 pos, u8 color);

u64 genDoublePushes(u32 *move_list, u8 pos, u8 color);
u64 genCastlingMoves(u32 *move_list, u8 pos, u8 color);
u64 genEnpassantMoves(u32 *move_list, u8 pos, u8 color);
u64 genPromotions(u32 *move_list, u8 pos, u8 color);

#endif






