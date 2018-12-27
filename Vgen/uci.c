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

#define INPUTBUFFER 400 * 6
#define NAME "Vgen"
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
    u8 fromRank;
    u8 toRank;
    u64 fromBB;
    u64 toBB;
    char fromFile;
    char toFile;
    int piece = -1;
    
    fromFile = *ptrChar;
    ptrChar++;
    fromRank = ((int)*ptrChar - '0') - 1;
    fromSq = (fromRank * 8) + addFileNumber(fromFile);
    fromBB = getBitboardFromSquare(fromSq);
    
    ptrChar++;
    toFile = *ptrChar;
    ptrChar++;
    toRank = ((int)*ptrChar - '0') - 1;
    toSq = (toRank * 8) + addFileNumber(toFile);
    toBB = getBitboardFromSquare(toSq);
    
	u32 moveList[MAX_MOVES];
	
	u8 moveCount = genMoves(moveList, sideToMove);
	
	for(int i = 0; i < moveCount; i++) {
		
		int fromSquare = from_sq(moveList[i]);
		int toSquare = to_sq(moveList[i]);
		
		if(fromSquare == fromSq && toSquare == toSq) {
			
			return moveList[i];
		}	
	}		
	
	
	/* 
    if(fromBB & pieceBB[sideToMove][PAWNS]) {
       
        piece = PAWNS;
    } else if(fromBB & pieceBB[sideToMove][KNIGHTS]) {
        
        piece = KNIGHTS;
    } else if (fromBB & pieceBB[sideToMove][BISHOPS]) {
        
        piece = BISHOPS;
    } else if (fromBB & pieceBB[sideToMove][ROOKS]) {
        
        piece = ROOKS;
    } else if (fromBB & pieceBB[sideToMove][QUEEN]) {
        
        piece = QUEEN;
    } else if (fromBB & pieceBB[sideToMove][KING]) {
        
        piece = KING;
    }
	
    if(piece == -1) {
        return 0;
    }
	
	if(piece == KING) {
	  // check for castling
		if(fromSq < 8 || fromSq > 55) {
            
			if(toSq < 8) {
                ptrChar--;
                // white castles
				if(*ptrChar == 'g') {
                    // white short castling
				
					return createMove(0, WHITE_CASTLE_KING_SIDE, MOVE_CASTLE, WHITE, ROOKS, KING, 4, 6);
				} else {
                    
					return createMove(0, WHITE_CASTLE_QUEEN_SIDE, MOVE_CASTLE, WHITE, ROOKS, KING, 4, 2);
				}
			} else {
                
				// black castles
				ptrChar--;
				if(*ptrChar == 'g') {
					
                    // white short castling
					return createMove(0, BLACK_CASTLE_KING_SIDE, MOVE_CASTLE, BLACK, ROOKS, KING, 60, 62);
                    
				} else {
                    
					return createMove(0, BLACK_CASTLE_QUEEN_SIDE, MOVE_CASTLE, BLACK, ROOKS, KING, 60, 58);
				}
			}
		}
	}
    
    
    if(piece == PAWNS) {
        
        // check for enpassant
        if(fromSq > 7 && fromSq < 56) {
    
            if((((sideToMove ? fromSq - 7 : fromSq + 7) == toSq) || (sideToMove ? fromSq - 9 : fromSq + 9) == toSq)
                && (toBB & empty)) {
                
                // A - file pawns
                if((fromSq % 8) == 0) {
                        
                    if(fromBB & pieceBB[sideToMove ^ 1][PAWNS]) {
                        // valid enpassant move
                        
                        return createMove(0, 0, MOVE_ENPASSANT, sideToMove, PAWNS, PAWNS, fromSq, toSq);
                    }
                } else if(((fromSq % 7)  - fromRank) == 0) {
                    // H - file pawns
                    
                    if(fromBB & pieceBB[sideToMove ^ 1][PAWNS]) {
                        // valid enpassant move
                        
                        return createMove(0, 0, MOVE_ENPASSANT, sideToMove, PAWNS, PAWNS, fromSq, toSq);
                    }
                } else{
                    // all pawns except A and H file
                    if((fromBB & pieceBB[sideToMove ^ 1][PAWNS]) ||
                        fromBB & pieceBB[sideToMove ^ 1][PAWNS]) {
                            // valid enpassant move
                        
                        return createMove(0, 0, MOVE_ENPASSANT, sideToMove, PAWNS, PAWNS, fromSq, toSq);
                    }
                }
            }
        }
    }
	
    if(piece == PAWNS) {
        if (toSq < 8 || toSq > 55) {
            
            u8 promType;
            
            ptrChar++;
            switch (*ptrChar) {
                case 'r':
                case 'R':
                    promType = PROMOTE_TO_ROOK;
                    break;
                case 'b':
                case 'B':
                    promType = PROMOTE_TO_BISHOP;
                    break;
                case 'n':
                case 'N':
                    promType = PROMOTE_TO_KNIGHT;
                    break;
                default:
                    promType = PROMOTE_TO_QUEEN;
                    break;
            }
            
            if(toBB & pieceBB[sideToMove ^ 1][PIECES]) {
            
                 if(toBB & pieceBB[sideToMove ^ 1][KNIGHTS]) {
                    
                    return createMove(promType, 0, MOVE_PROMOTION, sideToMove, KNIGHTS, piece, fromSq, toSq);
                } else if(toBB & pieceBB[sideToMove ^ 1][BISHOPS]) {
                    
                    return createMove(promType, 0, MOVE_PROMOTION, sideToMove, BISHOPS, piece, fromSq, toSq);
                } else if(toBB & pieceBB[sideToMove ^ 1][ROOKS]) {
                    
                    return createMove(promType, 0, MOVE_PROMOTION, sideToMove, ROOKS, piece, fromSq, toSq);
                } else if(toBB & pieceBB[sideToMove ^ 1][QUEEN]) {
                    
                    return createMove(promType, 0, MOVE_PROMOTION, sideToMove, QUEEN, piece, fromSq, toSq);
                }
            } else {
                
                return createMove(promType, 0, MOVE_PROMOTION, sideToMove, DUMMY, PAWNS, fromSq, toSq);
            }
        } else if(sideToMove ? ((fromSq - 16) == toSq) : ((fromSq + 16) == toSq)) {
            return createMove(0, 0, MOVE_DOUBLE_PUSH, sideToMove, DUMMY, PAWNS, fromSq, toSq);
        } else if(toBB & pieceBB[sideToMove ^ 1][PIECES]) {
            
            if(toBB & pieceBB[sideToMove ^ 1][PAWNS]) {
                
                return createMove(0, 0, MOVE_CAPTURE, sideToMove, PAWNS, PAWNS, fromSq, toSq);
            } else if(toBB & pieceBB[sideToMove ^ 1][KNIGHTS]) {
                
                return createMove(0, 0, MOVE_CAPTURE, sideToMove, KNIGHTS, PAWNS, fromSq, toSq);
            } else if(toBB & pieceBB[sideToMove ^ 1][BISHOPS]) {
                
                return createMove(0, 0, MOVE_CAPTURE, sideToMove, BISHOPS, PAWNS, fromSq, toSq);
            } else if(toBB & pieceBB[sideToMove ^ 1][ROOKS]) {
                
                return createMove(0, 0, MOVE_CAPTURE, sideToMove, ROOKS, PAWNS, fromSq, toSq);
            } else if(toBB & pieceBB[sideToMove ^ 1][QUEEN]) {
                
                return createMove(0, 0, MOVE_CAPTURE, sideToMove, QUEEN, PAWNS, fromSq, toSq);
            }
        } else {
            return createMove(0, 0, MOVE_NORMAL, sideToMove, DUMMY, PAWNS, fromSq, toSq);
        }
    } else {
    
		if(toBB & pieceBB[sideToMove ^ 1][PIECES]) {
            
			if(toBB & pieceBB[sideToMove ^ 1][PAWNS]) {
                
                return createMove(0, 0, MOVE_CAPTURE, sideToMove, PAWNS, piece, fromSq, toSq);
            } else if(toBB & pieceBB[sideToMove ^ 1][KNIGHTS]) {
                
                return createMove(0, 0, MOVE_CAPTURE, sideToMove, KNIGHTS, piece, fromSq, toSq);
            } else if(toBB & pieceBB[sideToMove ^ 1][BISHOPS]) {
                
                return createMove(0, 0, MOVE_CAPTURE, sideToMove, BISHOPS, piece, fromSq, toSq);
            } else if(toBB & pieceBB[sideToMove ^ 1][ROOKS]) {
                
                return createMove(0, 0, MOVE_CAPTURE, sideToMove, ROOKS, piece, fromSq, toSq);
            } else if(toBB & pieceBB[sideToMove ^ 1][QUEEN]) {
                
                return createMove(0, 0, MOVE_CAPTURE, sideToMove, QUEEN, piece, fromSq, toSq);
            }
        } else {
        
            return createMove(0, 0, MOVE_NORMAL, sideToMove, DUMMY, piece, fromSq, toSq);
        }
    } */
    
    return 0;
}

void parsePosition(char* lineIn) {
    
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
    
    ptrChar = strstr(lineIn, "moves");

    int count = 0;
    if(ptrChar != NULL) {

		u8 SIDE_TO_MOVE = WHITE;
        
		ptrChar += 6;
        while(*ptrChar) {
            // parse Move
            
            u32 move = parseMove(ptrChar, SIDE_TO_MOVE);
            
            if(move == 0) {
                break;
            }

            count++;
            make_move(move);
            ply = 0;
            
            while(*ptrChar && *ptrChar != ' ') {
                ptrChar++;
            }
            
            ptrChar++;
            SIDE_TO_MOVE ^= 1;
        }
    } else {
		
		// safe check if "moves" command is not present 
		color = WHITE;
	}
	
    printf("\nMove count - %d\n", count);
	
	if(count % 2 == 0) {
		color = WHITE;
	} else {
		color = BLACK;
	}
	
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
    
    char line[INPUTBUFFER];
    quit = false;
    
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    
    printf("id name %s\n", NAME);
    printf("id author Amar Thapa\n");
    printf("uciok\n");
    
    while (1) {
        memset(&line[0], 0, sizeof(line));
        fflush(stdout);
        
        if(!fgets(line, INPUTBUFFER, stdin)) {
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
        }
        
        if(quit) {
            break;
        }
    }
}
