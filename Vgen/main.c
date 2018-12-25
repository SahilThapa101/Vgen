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
#include "uci.h"

u8 COLOR;

void startPerft(u8 side, u8 depth);

void print(const char *str, size_t len, void *data) {
    printf("%.*s\n", (int) len, str);
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

int main(int argc, char **argv) {
    
    init_piece_bb();
    init_index_bb();
    init_king_attacks();
    init_knight_attacks();
    init_magic_moves();
    initCastleMaskAndFlags();
    initZobristKeys();
    
    UciLoop();
    
//    char* arg1 = argv[1];
//    u8 depth = 0;
//
//    if(argc > 2) {
//        depth = atoi(argv[2]);
//    }
//
//    printf("Depth = %d\n", depth);
//
//    ply = 0;
//
//    printf("\n");
//
//    const u8 SIDE_TO_MOVE = COLOR;
//
//    if(strcmp(arg1, "fen") == 0) {
//        if(argc < 2) {
//            printf("Fen string is blank\n");
//        } else {
//
//            char str[100];
//
//            if(argc > 2) {
//                strcpy(str, argv[2]);
//            }
//
//            if(argc > 3) {
//                strcat(str, " ");
//                strcat(str, argv[3]);
//            }
//
//            if(argc > 4) {
//                strcat(str, " ");
//                strcat(str, argv[4]);
//            }
//
//            if(argc > 5) {
//                strcat(str, " ");
//                strcat(str, argv[5]);
//            }
//
//            if(argc > 6) {
//                strcat(str, " ");
//                strcat(str, argv[6]);
//            }
//
//            if(argc > 7) {
//                strcat(str, " ");
//                strcat(str, argv[7]);
//            }
//
//            printf("fen entered is %s\n", str);
//        }
//    } else if(strcmp(arg1, "board") == 0) {
//
//        print_board(occupied);
//    } else if (strcmp(arg1, "perft") == 0) {
//
//        startPerft(SIDE_TO_MOVE, depth);
//    } else if (strcmp(arg1, "divide") == 0) {
//
//        divide(depth, SIDE_TO_MOVE);
//    } else if(strcmp(arg1, "evaluate") == 0) {
//
//        printf("Score is %d\n\n", evaluate(COLOR));
//    } else if (strcmp(arg1, "search") == 0) {
//
//        ply = 0;
//
//        search(COLOR);
//    }
    
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

