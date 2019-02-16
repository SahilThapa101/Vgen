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

u8 genMoves(Move *move_list, u8 color);
u8 genAttacksQuies(Move *moveList, u8 sideToMove);

u8 genPushes(Move *move_list, u8 pos, u8 color);
u8 genAttacks(Move *move_list, u8 pos, u8 color);
u8 genSpecialMoves(Move *moveList, u8 pos, u8 sideToMove);

u8 genKingPushes(Move *move_list, u8 pos, u8 color);
u8 genQueenPushes(Move *move_list, u8 pos, u8 color);
u8 genBishopPushes(Move *move_list, u8 pos, u8 color);
u8 genKnightPushes(Move *move_list, u8 pos, u8 color);
u8 genRookPushes(Move *move_list, u8 pos, u8 color);
u8 genPawnPushes(Move *move_list, u8 pos, u8 color);

u8 genKingAttacks(Move *move_list, u8 pos, u8 color);
u8 genQueenAttacks(Move *move_list, u8 pos, u8 color);
u8 genBishopAttacks(Move *move_list, u8 pos, u8 color);
u8 genKnightAttacks(Move *move_list, u8 pos, u8 color);
u8 genRookAttacks(Move *move_list, u8 pos, u8 color);
u8 genPawnAttacks(Move *move_list, u8 pos, u8 color);

u8 genDoublePushes(Move *move_list, u8 pos, u8 color);
u8 genCastlingMoves(Move *move_list, u8 pos, u8 color);
u8 genEnpassantMoves(Move *move_list, u8 pos, u8 color);
u8 genPromotionsNormal(Move *moveList, u8 pos, u8 sideToMove);
u8 genPromotionsAttacks(Move *move_list, u8 pos, u8 sideToMove);

#endif /* movegen_h */
