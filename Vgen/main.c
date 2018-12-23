//
//  main.c
//  Vgen
//
//  Created by Amar Thapa on 2/05/17.
//  Copyright © 2018 Amar Thapa. All rights reserved.
//

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
#include "evaluate.h"
#include "search.h"

u8 COLOR;

void startPerft(u8 side, u8 depth);

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
    
    FEN[0] = '[';
    FEN[1] = ch;
    
    while (ch != EOF) {
        ch = fgetc(fptr);
        FEN[len + 1] = ch;
        len++;
    }
    
    FEN[len] = ']';
    FEN[len + 1] = '\0';
    
    printf("FEN : %s\n", FEN);
    fclose(fptr);
}

void writeFen(char* str) {
    FILE *fptr;
    
    fptr = fopen("fen.txt", "w");
    if(fptr == NULL) {
        printf("Error!");
        exit(1);
    }
    
    fprintf(fptr,"%s", str);
    fclose(fptr);
}

void clearAllBitBoards() {
    
    pieceBB[WHITE][KING] &= 0;
    pieceBB[WHITE][QUEEN] &= 0;
    pieceBB[WHITE][BISHOPS] &= 0;
    pieceBB[WHITE][KNIGHTS] &= 0;
    pieceBB[WHITE][ROOKS] &= 0;
    pieceBB[WHITE][PAWNS] &= 0;
    pieceBB[WHITE][PIECES] &= 0;
    
    pieceBB[BLACK][KING] &= 0;
    pieceBB[BLACK][QUEEN] &= 0;
    pieceBB[BLACK][BISHOPS] &= 0;
    pieceBB[BLACK][KNIGHTS] &= 0;
    pieceBB[BLACK][ROOKS] &= 0;
    pieceBB[BLACK][PAWNS] &= 0;
    pieceBB[BLACK][PIECES] &= 0;
    
    //    print_bb(piece_bb[BLACK][KING]);
}

void splitTheFEN() {
    
    char str[1000];
    
    readFEN(str, 1);
    
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
    
    char* checkCastleFlags = ((char*) (d_array->buffer[2]));
    
    if (strcmp(checkCastleFlags, "-") != 0) {
        
        u32 len = strlen(checkCastleFlags);
        u8 flag = 0;
        
        for (int i = 0; i < len; i++) {
            if (checkCastleFlags[i] == 'K') {
                
                flag |= CastleFlagWhiteKing;
            } else if (checkCastleFlags[i] == 'k') {
                
                flag |= CastleFlagBlackKing;
            } else if (checkCastleFlags[i] == 'Q') {
                
                flag |= CastleFlagWhiteQueen;
            } else if (checkCastleFlags[i] == 'q') {
                
                flag |= CastleFlagBlackQueen;
            }
        }
        
        moveStack[0].castleFlags = flag;
        moveStack[0].prevCastleFlags = flag;
    }
    
    char* checkEpSquare = ((char*) (d_array->buffer[3]));
    
    if (checkEpSquare[0] != '-') {
        moveStack[0].epFlag = 1;
        moveStack[0].epSquare = bbFromAlgebricPos(checkEpSquare);
    }
    
    printf("Side to move : %s\n", COLOR == 0 ? "WHITE" : "BLACK");
    
    for (int i = 0; i < d_array->count; i++) {
        split(d_array->buffer[i], '/', initFEN, mainFEN);
    }
    
    clearAllBitBoards();
    
    int pos = 63;
    
    for (int i = 0; i < 8; i++) {
        
        char reverse[10];
        int reversePos = 0;
        
        for (u32 j = strlen((char*) mainFEN->buffer[i]); j > 0; j--) {
            char ch = ((char*) mainFEN->buffer[i])[j - 1];
            
            reverse[reversePos] = ch;
            reversePos++;
        }
        
        reverse[reversePos] = '\0';
        
        reversePos = 0;
        
        while (reverse[reversePos] != '\0') {
            //printf("%c ", ((char*)(mainFEN->buffer[(mainFEN->count - 1) - i]))[pos]);
            switch (reverse[reversePos]) {
                    
                    //Black side
                case 'k':
                    pieceBB[BLACK][KING] |= getBitboardFromSquare(pos);
                    pos--;
                    //        printf("%s\n", "White King");
                    break;
                case 'q':
                    pieceBB[BLACK][QUEEN] |= getBitboardFromSquare(pos);
                    pos--;
                    //        printf("%s\n", "White Queen");
                    break;
                case 'b':
                    pieceBB[BLACK][BISHOPS] |= getBitboardFromSquare(pos);
                    pos--;
                    //        printf("%s\n", "White Bishop");
                    break;
                case 'n':
                    pieceBB[BLACK][KNIGHTS] |= getBitboardFromSquare(pos);
                    pos--;
                    //        printf("%s\n", "White Knight");
                    break;
                case 'r':
                    pieceBB[BLACK][ROOKS] |= getBitboardFromSquare(pos);
                    pos--;
                    //        printf("%s\n", "White Rook");
                    break;
                case 'p':
                    pieceBB[BLACK][PAWNS] |= getBitboardFromSquare(pos);
                    pos--;
                    //        printf("%s\n", "White Pawns");
                    break;
                    
                    //White side
                case 'K':
                    pieceBB[WHITE][KING] |= getBitboardFromSquare(pos);
                    pos--;
                    //        printf("%s\n", "Black King");
                    break;
                case 'Q':
                    pieceBB[WHITE][QUEEN] |= getBitboardFromSquare(pos);
                    pos--;
                    //        printf("%s\n", "Black Queen");
                    break;
                case 'B':
                    pieceBB[WHITE][BISHOPS] |= getBitboardFromSquare(pos);
                    pos--;
                    //        printf("%s\n", "Black Bishop");
                    break;
                case 'N':
                    pieceBB[WHITE][KNIGHTS] |= getBitboardFromSquare(pos);
                    pos--;
                    //        printf("%s\n", "Black Knight");
                    break;
                case 'R':
                    pieceBB[WHITE][ROOKS] |= getBitboardFromSquare(pos);
                    pos--;
                    //        printf("%s\n", "Black Rook");
                    break;
                case 'P':
                    pieceBB[WHITE][PAWNS] |= getBitboardFromSquare(pos);
                    pos--;
                    //        printf("%s\n", "Black Pawns");
                    break;
                    
                    //    No of spaces
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
    
    pieceBB[WHITE][PIECES] = pieceBB[WHITE][KING] | pieceBB[WHITE][QUEEN]
    | pieceBB[WHITE][BISHOPS] | pieceBB[WHITE][KNIGHTS]
    | pieceBB[WHITE][ROOKS] | pieceBB[WHITE][PAWNS];
    pieceBB[BLACK][PIECES] = pieceBB[BLACK][KING] | pieceBB[BLACK][QUEEN]
    | pieceBB[BLACK][BISHOPS] | pieceBB[BLACK][KNIGHTS]
    | pieceBB[BLACK][ROOKS] | pieceBB[BLACK][PAWNS];
    occupied = pieceBB[BLACK][KING] | pieceBB[BLACK][QUEEN]
    | pieceBB[BLACK][BISHOPS] | pieceBB[BLACK][KNIGHTS]
    | pieceBB[BLACK][ROOKS] | pieceBB[BLACK][PAWNS]
    | pieceBB[WHITE][KING] | pieceBB[WHITE][QUEEN]
    | pieceBB[WHITE][BISHOPS] | pieceBB[WHITE][KNIGHTS]
    | pieceBB[WHITE][ROOKS] | pieceBB[WHITE][PAWNS];
    
    empty = ~occupied;
    
    dynarray_for_each(d_array, free);
    dynarray_delete(d_array);
    dynarray_for_each(mainFEN, free);
    dynarray_delete(mainFEN);
}

void initCastleMaskAndFlags() {
    
    for (int i = 0; i < 64; i++) {
        rookCastleFlagMask[i] = 15;
    }
    
    rookCastleFlagMask[0] ^= CastleFlagWhiteQueen;
    rookCastleFlagMask[7] ^= CastleFlagWhiteKing;
    rookCastleFlagMask[56] ^= CastleFlagBlackQueen;
    rookCastleFlagMask[63] ^= CastleFlagBlackKing;
}

void fen(char* str) {
    
    writeFen(str);
}

int main(int argc, char **argv) {
    
    init_piece_bb();
    init_index_bb();
    init_king_attacks();
    init_knight_attacks();
    init_magic_moves();
    initCastleMaskAndFlags();
    initZobristKeys();
    
    splitTheFEN();
    
    char* arg1 = argv[1];
    u8 depth = 0;
    
    if(argc > 2) {
        depth = atoi(argv[2]);
    }
    
    printf("Depth = %d\n", depth);
    
    ply = 0;
    
    printf("\n");
    
    const u8 SIDE_TO_MOVE = COLOR;
    
    if(strcmp(arg1, "fen") == 0) {
        if(argc < 2) {
            printf("Fen string is blank\n");
        } else {
            
            char str[100];
            
            if(argc > 2) {
                strcpy(str, argv[2]);
            }
            
            if(argc > 3) {
                strcat(str, " ");
                strcat(str, argv[3]);
            }
            
            if(argc > 4) {
                strcat(str, " ");
                strcat(str, argv[4]);
            }
            
            if(argc > 5) {
                strcat(str, " ");
                strcat(str, argv[5]);
            }
            
            if(argc > 6) {
                strcat(str, " ");
                strcat(str, argv[6]);
            }
            
            if(argc > 7) {
                strcat(str, " ");
                strcat(str, argv[7]);
            }
            
            printf("fen entered is %s\n", str);
            writeFen(str);
        }
    } else if(strcmp(arg1, "board") == 0) {
        
        print_board(occupied);
    } else if (strcmp(arg1, "perft") == 0) {
        
        startPerft(SIDE_TO_MOVE, depth);
    } else if (strcmp(arg1, "divide") == 0) {
        
        divide(depth, SIDE_TO_MOVE);
    } else if(strcmp(arg1, "evaluate") == 0) {
        
        printf("Score is %d\n\n", evaluate(COLOR));
    } else if (strcmp(arg1, "search") == 0) {
        
        ply = 0;
        
        search(COLOR);
    }
    
    return 0;
}

void startPerft(u8 side, u8 depth) {
    
    u64 nodes;
    
    clock_t start, end;
    double cpu_time_used;
    double nps;
    
    prevCap = 0;
    prevEp = 0;
    prevCas = 0;
    
    for (int i = 1; i <= depth; i++) {
        
        nodes = 0;
        quiet = 0;
        cap = 0;
        ep = 0;
        cas = 0;
        check = 0;
        ply = 0;
        prom = 0;
        
        struct hist defaultHist = { 0, 0, 0, 0 };
        
        hist[ply] = defaultHist;
        
        start = clock();
        
        nodes = perft(i, side);
        
        end = clock();
        
        cap = cap - prevCap;
        ep = ep - prevEp;
        cas = cas - prevCas;
        
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        nps = (double) (nodes / (cpu_time_used * 1000000));
        printf("Depth(%d)=   ", i);
        printf(
               "%10llu (%8.3f sec), color - %s, captures - %8llu, en - %6llu, cas - %6llu, prom - %8llu, %7.3f MN/s\n",
               nodes, cpu_time_used, ((side == 0) ? "WHITE" : "BLACK"), cap,
               ep, cas, prom, nps);
        
        prevCap = cap;
        prevEp = ep;
        prevCas = cas;
    }
}

