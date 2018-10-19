/*
 * Main.c
 *
 *  Created on: May 2, 2017
 *      Author: Amar Thapa
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "utility.h"
#include "magicmoves.h"
#include "nonslidingmoves.h"
#include "init.h"
#include "perft.h"
#include "dynarray.h"

u8 COLOR;

typedef void (*split_fn)(const char *, size_t, void *);

void split(const char *str, char sep, split_fn fun, void *data) {
	unsigned int start = 0, stop;
	for (stop = 0; str[stop]; stop++) {
		if (str[stop] == sep) {
			fun(str + start, stop - start, data);
			start = stop + 1;
		}
	}
	fun(str + start, stop - start, data);
}

void initFEN(const char *str, size_t len, void *data) {
	dynarray *array = data;
	char *token = calloc(len + 1, 1);
	memcpy(token, str, len);
	dynarray_add_tail(array, token);
}

void print(const char *str, size_t len, void *data) {
	printf("%.*s\n", (int) len, str);
}

void readFEN(char* FEN, int len) {
	FILE *fptr;
	char ch;

	fptr = fopen("fen.txt", "r");

	if (fptr == NULL) {
		printf("Cannot open file for FEN strings \n");
	}

	ch = fgetc(fptr);

	while (ch != EOF) {
		ch = fgetc(fptr);
		FEN[len] = ch;
		len++;
	}

	FEN[len] = '\0';

	printf("%s\n", FEN);
	fclose(fptr);
}

void clearAllBitBoards() {

	piece_bb[WHITE][KING] &= 0;
	piece_bb[WHITE][QUEEN] &= 0;
	piece_bb[WHITE][BISHOPS] &= 0;
	piece_bb[WHITE][KNIGHTS] &= 0;
	piece_bb[WHITE][ROOKS] &= 0;
	piece_bb[WHITE][PAWNS] &= 0;
	piece_bb[WHITE][PIECES] &= 0;

	piece_bb[BLACK][KING] &= 0;
	piece_bb[BLACK][QUEEN] &= 0;
	piece_bb[BLACK][BISHOPS] &= 0;
	piece_bb[BLACK][KNIGHTS] &= 0;
	piece_bb[BLACK][ROOKS] &= 0;
	piece_bb[BLACK][PAWNS] &= 0;
	piece_bb[BLACK][PIECES] &= 0;

//	print_bb(piece_bb[BLACK][KING]);
}

void splitTheFEN() {

	int len = 0;
	char str[100];

	readFEN(str, len);

	dynarray *d_array = dynarray_create(0);
	dynarray *mainFEN = dynarray_create(0);

	split(str, ' ', initFEN, d_array);

	switch (((char*) (d_array->buffer[1]))[0]) {
	case 'w':
		COLOR = WHITE;
		break;
	case 'b':
		COLOR = BLACK;
		break;
	default:
		COLOR = WHITE;
		break;
	}

//	char* checkEpSquare = ((char*) (d_array->buffer[3]));
//
//	if(checkEpSquare[0] != '-') {
//		epFlag = 1;
//		epSquare = 0x0000080000000000U;
//		printf("%s, %10llu", checkEpSquare, (unsigned long long)epSquare);
//	}

	printf("Side to move : %s\n", COLOR == 0 ? "WHITE" : "BLACK");

	for (int i = 0; i < d_array->count; i++) {
		split(d_array->buffer[i], '/', initFEN, mainFEN);
	}

	clearAllBitBoards();

	int pos = 63;

	for (int i = 0; i < 8; i++) {

		char reverse[10];
		int reversePos = 0;

		for (int j = strlen((char*) mainFEN->buffer[i]); j > 0; j--) {
			char ch = ((char*) mainFEN->buffer[i])[j - 1];

			reverse[reversePos] = ch;
			reversePos++;
		}

		reverse[reversePos] = '\0';

		reversePos = 0;
		while (reverse[reversePos] != NULL) {
			//printf("%c ", ((char*)(mainFEN->buffer[(mainFEN->count - 1) - i]))[pos]);
			switch (reverse[reversePos]) {

			//Black side
			case 'k':
				piece_bb[BLACK][KING] |= returnBB(pos);
				pos--;
				//		printf("%s\n", "White King");
				break;
			case 'q':
				piece_bb[BLACK][QUEEN] |= returnBB(pos);
				pos--;
				//		printf("%s\n", "White Queen");
				break;
			case 'b':
				piece_bb[BLACK][BISHOPS] |= returnBB(pos);
				pos--;
				//		printf("%s\n", "White Bishop");
				break;
			case 'n':
				piece_bb[BLACK][KNIGHTS] |= returnBB(pos);
				pos--;
				//		printf("%s\n", "White Knight");
				break;
			case 'r':
				piece_bb[BLACK][ROOKS] |= returnBB(pos);
				pos--;
				//		printf("%s\n", "White Rook");
				break;
			case 'p':
				piece_bb[BLACK][PAWNS] |= returnBB(pos);
				pos--;
				//		printf("%s\n", "White Pawns");
				break;

				//White side
			case 'K':
				piece_bb[WHITE][KING] |= returnBB(pos);
				pos--;
				//		printf("%s\n", "Black King");
				break;
			case 'Q':
				piece_bb[WHITE][QUEEN] |= returnBB(pos);
				pos--;
				//		printf("%s\n", "Black Queen");
				break;
			case 'B':
				piece_bb[WHITE][BISHOPS] |= returnBB(pos);
				pos--;
				//		printf("%s\n", "Black Bishop");
				break;
			case 'N':
				piece_bb[WHITE][KNIGHTS] |= returnBB(pos);
				pos--;
				//		printf("%s\n", "Black Knight");
				break;
			case 'R':
				piece_bb[WHITE][ROOKS] |= returnBB(pos);
				pos--;
				//		printf("%s\n", "Black Rook");
				break;
			case 'P':
				piece_bb[WHITE][PAWNS] |= returnBB(pos);
				pos--;
				//		printf("%s\n", "Black Pawns");
				break;

				//	No of spaces
			case '1':
				pos = pos - 1;
				break;
			case '2':
				pos = pos - 2;
				break;
			case '3':
				pos = pos - 3;
				break;
			case '4':
				pos = pos - 4;
				break;
			case '5':
				pos = pos - 5;
				break;
			case '6':
				pos = pos - 6;
				break;
			case '7':
				pos = pos - 7;
				break;
			case '8':
				pos = pos - 8;
				break;
			default: //printf("%s\n", "Blank Space");
				break;

			}
			reversePos++;
		}
	}

	piece_bb[WHITE][PIECES] = piece_bb[WHITE][KING] | piece_bb[WHITE][QUEEN]
			| piece_bb[WHITE][BISHOPS] | piece_bb[WHITE][KNIGHTS]
			| piece_bb[WHITE][ROOKS] | piece_bb[WHITE][PAWNS];
	piece_bb[BLACK][PIECES] = piece_bb[BLACK][KING] | piece_bb[BLACK][QUEEN]
			| piece_bb[BLACK][BISHOPS] | piece_bb[BLACK][KNIGHTS]
			| piece_bb[BLACK][ROOKS] | piece_bb[BLACK][PAWNS];
	occupied = piece_bb[BLACK][KING] | piece_bb[BLACK][QUEEN]
			| piece_bb[BLACK][BISHOPS] | piece_bb[BLACK][KNIGHTS]
			| piece_bb[BLACK][ROOKS] | piece_bb[BLACK][PAWNS]
			| piece_bb[WHITE][KING] | piece_bb[WHITE][QUEEN]
			| piece_bb[WHITE][BISHOPS] | piece_bb[WHITE][KNIGHTS]
			| piece_bb[WHITE][ROOKS] | piece_bb[WHITE][PAWNS];

	empty = ~occupied;

	print_board(occupied);

	dynarray_for_each(d_array, free);
	dynarray_delete(d_array);
	dynarray_for_each(mainFEN, free);
	dynarray_delete(mainFEN);
}

int main(int argc, char **argv) {

	init_piece_bb();
	init_index_bb();
	init_king_attacks();
	init_knight_attacks();
	init_magic_moves();

	splitTheFEN();

	u8 depth = atoi(argv[1]);

	printf("Depth = %d\n", depth);

	double diff;

	ply = 0;

	hist_add.move = 0;
	hist_add.ep_flag = 0;
	hist_add.castle_flags = (u8) 0x1111;

	hist[ply] = hist_add;

	printf("\n");

	const u8 SIDE_TO_MOVE = COLOR;

	//divide(depth, SIDE_TO_MOVE);
	startPerft(SIDE_TO_MOVE, depth);

	return 0;
}

void startPerft(u8 side, u8 depth)  {

	u8 i;
	u64 nodes;

	clock_t start1, end1;
	double cpu_time_used;
	double nps;

	for (int i = 1; i <= depth; i++) {

		nodes = 0;
		quiet = 0;
		cap = 0;
		en = 0;
		cas = 0;
		check = 0;
		ply = 0;

		epFlag = 0;

		struct hist defaultHist = { 0, 0, 0, 0 };

		hist[ply] = defaultHist;

		start1 = clock();

		nodes = perft(i, side);

		end1 = clock();

		cpu_time_used = ((double) (end1 - start1)) / CLOCKS_PER_SEC;
		nps = (double)(nodes / (cpu_time_used * 1000000));
		printf("Depth(%d)=   ", i);
		printf(
				"%10llu (%8.3f sec), color - %s, captures - %8llu, en - %6llu, cas - %6llu, checks - %8llu, %7.3f MN/s\n",
				nodes, cpu_time_used, ((side == 0) ? "WHITE" : "BLACK"), cap, en, cas, check, nps);
	}
}

