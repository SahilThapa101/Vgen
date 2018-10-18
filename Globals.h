/*
 * Globals.h
 *
 *  Created on: May 2, 2017
 *      Author: Amar Thapa
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <stdbool.h>
#include <stdio.h>

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long int u32;
typedef unsigned long long int u64;

#define C64(constantU64) constantU64

#define MAX_MOVES 256
#define MAX_DEPTH 2

#define WHITE 0
#define BLACK 1

u8 color;

u64 quiet, cap, en, cas, check;

/* pieceBB is an array containing bitboards for all pieces */

u64 piece_bb[2][7]; /* color * (piece_type + pieces of that color) = 2 * 7 = 14 */

/* array size for indexBB array */
#define INDEX_BB_SIZE 64

u64 index_bb[INDEX_BB_SIZE];

/********************************/

#define MAX_PLY 1024
u32 ply;

struct hist {
	u64 move;
	u64 ep_sq;
	u8 castle_flags;
	u8 ep_flag;
} hist[MAX_PLY], hist_add;

/********************************/
u8 castling_rights[2];

/********************************/

/* index for pieces in pieceBB array */

#define KING 0
#define QUEEN 1
#define BISHOPS 2
#define KNIGHTS 3
#define ROOKS 4
#define PAWNS 5
#define PIECES 6
#define DUMMY 7

/* occupied and empty squares bb */

u64 occupied, empty;

/* Extract data from a move structure */
#define prom_type(move)   		(move & 0x3000000) >> 24
#define castle_dir(move)		(move & 0xC00000) >> 22
#define move_type(move)         (move & 0x380000) >> 19
#define color_type(move)        (move & 0x40000) >> 18
#define c_piece_type(move)      (move & 0x38000) >> 15
#define piece_type(move)	    (move & 0x7000) >> 12  	
#define from_sq(move)          	(move & 0xFC0) >> 6
#define to_sq(move)				move & 0x3F

#define RANK_4 0x00000000FF000000U
#define RANK_5 0x000000FF00000000U

#define RANK_2 0x000000000000FF00U
#define RANK_7 0x00FF000000000000U

#define WQ_SIDE_SQS 0x000000000000000EU
#define WK_SIDE_SQS 0x0000000000000060U
#define BQ_SIDE_SQS 0x0E00000000000000U
#define BK_SIDE_SQS 0x6000000000000000U

#define NOT_A_FILE 0XFEFEFEFEFEFEFEFEU
#define NOT_H_FILE 0X7F7F7F7F7F7F7F7FU

#endif /* GLOBALS_H_ */
