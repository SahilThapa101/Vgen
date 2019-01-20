//
//  perft.c
//  Vgen
//
//  Created by Amar Thapa on 23/12/18.
//  Copyright © 2018 Amar Thapa. All rights reserved.
//

#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "perft.h"
#include "movegen.h"
#include "make_unmake.h"
#include "utility.h"


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
        
        start = clock();
        
        nodes = perft(i, side);
        
        end = clock();
        
        cap = cap - prevCap;
        ep = ep - prevEp;
        cas = cas - prevCas;
        
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        nps = (double) (nodes / (cpu_time_used * 1000000));
        printf("Depth(%d)=   ", i);
        printf("%10llu (%8.3f sec), color - %s, captures - %8llu, en - %6llu, cas - %6llu, prom - %8llu, %7.3f MN/s\n",
               nodes, cpu_time_used, ((side == 0) ? "WHITE" : "BLACK"), cap, ep, cas, prom, nps);
        
        prevCap = cap;
        prevEp = ep;
        prevCas = cas;
    }
}

u64 perft(u8 depth, u8 color) {
    
    Move move_list[MAX_MOVES];
    u64 nodes = 0;
    u8 n_moves;
    u8 i;
    u8 sideToMove = color;
    
    if (depth == 0) {
        return 1;
    }
    
    ply = ply + 1;
    moveStack[ply].epFlag = moveStack[ply - 1].epFlag;
    moveStack[ply].epSquare = moveStack[ply - 1].epSquare;
    moveStack[ply].castleFlags  =  moveStack[ply - 1].castleFlags;
    
    //    printf("%x, ", moveStack[ply].castleFlags);
    
    n_moves = genMoves(move_list, sideToMove);
    
	Move move;
    for (i = 0; i < n_moves; i++) {
        
		move = move_list[i];
        make_move(move.move);
        
        if (isKingInCheck(color)) {
            // illegal move
            if (move_type(move.move) == 4) {
                cas--;
            }
            if (move_type(move.move) == 1) {
                cap--;
            }
            if (move_type(move.move) == 3) {
                ep--;
            }
            if (move_type(move.move) == 5) {
                prom--;
            }
        } else {
            
            nodes += perft(depth - 1, (sideToMove ^ 1));
        }
        
        unmake_move(move.move);
    }
    
    ply = ply - 1;
    
    return nodes;
}
