//
//  uci.c
//  Vgen
//
//  Created by Amar Thapa on 25/12/18.
//  Copyright © 2018 Amar Thapa. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "uci.h"
#include "utility.h"
#include "make_unmake.h"
#include "fen.h"
#include "movegen.h"
#include "vtime.h"
#include "search.h"
#include "perft.h"
#include "hash.h"

#define NAME "Vgen_20190125"
#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

int addFileNumber(char ch) {
    
    switch (ch) {
        case 'a':
            return 0;
        case 'b':
            return 1;
        case 'c':
            return 2;
        case 'd':
            return 3;
        case 'e':
            return 4;
        case 'f':
            return 5;
        case 'g':
            return 6;
        case 'h':
            return 7;
    }
    
    return 0;
}

void parseFen(char* lineIn) {
    
    splitTheFEN(lineIn);
}

// e2e4
u32 parseMove(char *ptrChar, u8 sideToMove) {
   
    if(*ptrChar == (char)' ' || *ptrChar == (char)0) {
        return 0;
    }
    
    // 0 - 63
    u8 fromSq;
    u8 toSq;
    char fromFile;
    char toFile;
    
    fromFile = *ptrChar;
    ptrChar++;
    fromSq = ((((int)*ptrChar - '0') - 1) * 8) + addFileNumber(fromFile);
    
    ptrChar++;
    
	toFile = *ptrChar;
    ptrChar++;
    toSq = ((((int)*ptrChar - '0') - 1) * 8) + addFileNumber(toFile);
    
	Move moveList[MAX_MOVES];
	
	u8 moveCount = genMoves(moveList, sideToMove);
	
	u32 move;
	for(int i = 0; i < moveCount; i++) {
		
		int fromSquare = from_sq(moveList[i].move);
		int toSquare = to_sq(moveList[i].move);
		
		if(fromSquare == fromSq && toSquare == toSq) {
			
			return moveList[i].move;
		}	
	}		
	
	//debugSEE(move, sideToMove);

    return 0;
}

void parsePosition(char* lineIn) {

	clearKillerMovesTable();
	clearHistoryTable();
	clearHashTable();
	clearRepetitionHashTable();

    lineIn += 9;
    char *ptrChar = lineIn;
    if(strncmp(lineIn, "startpos", 8) == 0) {
        
        parseFen(START_FEN);
    } else {
 
		ptrChar = strstr(lineIn, "fen");
        if(ptrChar == NULL) {
            
            parseFen(START_FEN);
        } else {
            
            ptrChar += 4;
            parseFen(ptrChar);
        }
    }
	
	initHashKey();
    
	ptrChar = strstr(lineIn, "moves");
	
	u8 sideToMove = WHITE;

	if(ptrChar != NULL) {
	
		ptrChar += 6;
        while(*ptrChar) {

            // parse Move
            u32 move = parseMove(ptrChar, sideToMove);
            
            if(move == 0) {
                break;
            }

            // add game history hashKeys to repetitions hash table
            insertRepetitionHashKey(hashKey, sideToMove);
            make_move(move);

            ply = 0;
            
            sideToMove ^= 1;
			
            while(*ptrChar && *ptrChar != ' ') {
                ptrChar++;
            }
            
            ptrChar++;
        }
    }
	
	color = sideToMove;
	
    print_board(occupied);
}

void parseGo(char *lineIn,  u8 sideToMove) {
    
    int depthCurrent = -1;
    int movesToGo = 30;
    int moveTime = -1;
    int time = -1;
    int inc = 0;
    
    char * ptr = NULL;
    
    if((ptr = strstr(lineIn, "INF")));
    
    if((ptr = strstr(lineIn, "binc")) && sideToMove == BLACK) {
        
        inc = atoi(ptr + 5);
    }
    
    if((ptr = strstr(lineIn, "winc")) && sideToMove == WHITE) {
        
        inc = atoi(ptr + 5);
    }
    
    if((ptr = strstr(lineIn, "btime")) && sideToMove == BLACK) {
        
        time = atoi(ptr + 6);
    }

    if((ptr = strstr(lineIn, "wtime")) && sideToMove == WHITE) {
        
        time = atoi(ptr + 6);
    }

    if((ptr = strstr(lineIn, "movestogo"))) {
        
        movesToGo = atoi(ptr + 10);
    }
    
    if((ptr =  strstr(lineIn, "movetime"))) {
        
        moveTime = atoi(ptr + 9);
    }

    if((ptr = strstr(lineIn, "depth"))) {
        
        depthCurrent = atoi(ptr + 6);
    }
    
    if(moveTime != -1) {
        time = moveTime;
        movesToGo = 1;
    }
    
    startTime = getTimeMs();
    
    if(time != -1) {
        
        timeSet = true;
        time /= movesToGo;
        time -= 50;
        stopTime = startTime + time + inc;
    }
    
    if(depthCurrent == -1) {
        depthCurrent = MAX_DEPTH;
    }
    
    depth = depthCurrent;
    
    printf("time:%d start:%llu stop:%llu depth:%d timeset:%d\n", time, startTime, stopTime, depth, timeSet);
    
    search(sideToMove);
}

void UciLoop() {
    
    char line[INPUT_BUFFER];
    quit = false;
    
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    
    printf("id name %s\n", NAME);
    printf("id author Amar Thapa\n");
    printf("uciok\n");
    
    while (1) {
        memset(&line[0], 0, sizeof(line));
        fflush(stdout);
        
        if(!fgets(line, INPUT_BUFFER, stdin)) {
            continue;
        }
        
        if(line[0] == '\n') {
           continue;
        }
        
        if (!strncmp(line, "isready", 7)) {
            printf("readyok\n");
            continue;
        } else if(!strncmp(line, "position", 8)) {
            color = WHITE;
            
			moveStack[0].castleFlags = CastleFlagWhiteKing | CastleFlagWhiteQueen | CastleFlagBlackKing | CastleFlagBlackQueen;
			moveStack[0].prevCastleFlags = moveStack[0].castleFlags;
			moveStack[0].epFlag = 0;
			ply = 0;
			
			parsePosition(line);
        } else if(!strncmp(line, "ucinewgame", 10)) {
            color = WHITE;
			    
			moveStack[0].castleFlags = CastleFlagWhiteKing | CastleFlagWhiteQueen | CastleFlagBlackKing | CastleFlagBlackQueen;
			moveStack[0].prevCastleFlags = moveStack[0].castleFlags;
			moveStack[0].epFlag = 0;
			ply = 0;
			
            parsePosition("position startpos\n");
        } else if(!strncmp(line, "go", 2)) {
           
		    parseGo(line, color);
        } else if(!strncmp(line, "quit", 4)) {
          
            quit = true;
            break;
        } else if(!strncmp(line, "uci", 3)) {
            printf("id name %s\n", NAME);
            printf("id author Amar Thapa\n");
            printf("uciok\n");
        } else if(!strncmp(line, "perft", 5)) {
			
			startPerft(color, 7);
		}
        if(quit) {
            break;
        }
    }
}
