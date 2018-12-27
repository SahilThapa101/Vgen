//
//  movegen.h
//  Vgen
//
//  Created by Amar Thapa on 23/12/18.
//  Copyright © 2018 Amar Thapa. All rights reserved.
//

#ifndef movegen_h
#define movegen_h

/* encode the move in a 32 bit integer */

u32 createMove(u8 promotion_type, u8 castle_dir, u8 move_type, u8 color, u8 c_piece, u8 piece, u8 from, u8 to);

u8 genCaptures(u32 *move_list, u8 color);

u8 genMoves(u32 *move_list, u8 color);

u8 genPushes(u32 *move_list, u8 pos, u8 color);
u8 genAttacks(u32 *move_list, u8 pos, u8 color);
u8 genSpecialMoves(u32 *move_list, u8 pos, u8 color);

u8 genKingPushes(u32 *move_list, u8 pos, u8 color);
u8 genQueenPushes(u32 *move_list, u8 pos, u8 color);
u8 genBishopPushes(u32 *move_list, u8 pos, u8 color);
u8 genKnightPushes(u32 *move_list, u8 pos, u8 color);
u8 genRookPushes(u32 *move_list, u8 pos, u8 color);
u8 genPawnPushes(u32 *move_list, u8 pos, u8 color);

u8 genKingAttacks(u32 *move_list, u8 pos, u8 color);
u8 genQueenAttacks(u32 *move_list, u8 pos, u8 color);
u8 genBishopAttacks(u32 *move_list, u8 pos, u8 color);
u8 genKnightAttacks(u32 *move_list, u8 pos, u8 color);
u8 genRookAttacks(u32 *move_list, u8 pos, u8 color);
u8 genPawnAttacks(u32 *move_list, u8 pos, u8 color);

u8 genDoublePushes(u32 *move_list, u8 pos, u8 color);
u8 genCastlingMoves(u32 *move_list, u8 pos, u8 color);
u8 genEnpassantMoves(u32 *move_list, u8 pos, u8 color);
u8 genPromotions(u32 *move_list, u8 pos, u8 color);

#endif /* movegen_h */
