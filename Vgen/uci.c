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
    
    writeFen(lineIn);
    splitTheFEN();
}

// e2e4
u32 parseMove(char *ptrChar, u8 sideToMove) {
   
    if(*ptrChar == (char)' ' || *ptrChar == (char)0) {
        return 0;
    }
    
    u8 piece;
    u8 from;
    u8 to;
    u8 fromRank;

    switch (*ptrChar) {
        case 'k':
        case 'K':
            piece = KING;
            break;
        case 'q':
        case 'Q':
            piece = QUEEN;
            break;
        case 'r':
        case 'R':
            piece = ROOKS;
            break;
        case 'b':
        case 'B':
            piece = BISHOPS;
            break;
        case 'n':
        case 'N':
            piece = KNIGHTS;
            break;
        default:
            piece = PAWNS;
            break;
    }
    
    if(piece == PAWNS) {
        char fromFile = *ptrChar;
        
        ptrChar++;
        if(*ptrChar == (char)' ' || *ptrChar == (char)0) {
            return 0;
        }
        
        from = (int)*ptrChar - '0';
        fromRank = from;
        from = ((from - 1) * 8) + addFileNumber(fromFile);
        
        ptrChar++;
        
        char toFile = *ptrChar;
        ptrChar++;
        to = (int)*ptrChar - '0';
        to = ((to - 1) * 8) + addFileNumber(toFile);
        
        // check for enpassant
        if(from > 7 && from < 56) {
    
            if((((sideToMove ? from - 7 : from + 7) == to) || (sideToMove ? from - 9 : from + 9) == to)
                && (getBitboardFromSquare(to) & empty)) {
                
                // A - file pawns
                if((from % 8) == 0) {
                        
                    if(getBitboardFromSquare(from - 1) & pieceBB[sideToMove ^ 1][PAWNS]) {
                        // valid enpassant move
                        
                        return createMove(0, 0, MOVE_ENPASSANT, sideToMove, PAWNS, PAWNS, from, to);
                    }
                } else if(((from % 7)  - (fromRank - 1)) == 0) {
                    // H - file pawns
                    
                    if(getBitboardFromSquare(from + 1) & pieceBB[sideToMove ^ 1][PAWNS]) {
                        // valid enpassant move
                        
                        return createMove(0, 0, MOVE_ENPASSANT, sideToMove, PAWNS, PAWNS, from, to);
                    }
                } else{
                    // all pawns except A and H file
                    if((getBitboardFromSquare(from - 1) & pieceBB[sideToMove ^ 1][PAWNS]) ||
                        getBitboardFromSquare(from + 1) & pieceBB[sideToMove ^ 1][PAWNS]) {
                            // valid enpassant move
                        
                        return createMove(0, 0, MOVE_ENPASSANT, sideToMove, PAWNS, PAWNS, from, to);
                    }
                }
            }
        }
        
        // check for castling
        if(from == 1 || from == 8) {
            
            if(to == 1) {
                
                // white castles
                ptrChar--;
                if(*ptrChar == 'g') {
                    // white short castling
                    return createMove(0, WHITE_CASTLE_KING_SIDE, MOVE_CASTLE, sideToMove, ROOKS, KING, from, to);
                    
                } else {
                    
                    return createMove(0, WHITE_CASTLE_QUEEN_SIDE, MOVE_CASTLE, sideToMove, ROOKS, KING, from, to);
                }
            } else {
                
                // black castles
                ptrChar--;
                if(*ptrChar == 'g') {
                    // white short castling
                    return createMove(0, BLACK_CASTLE_KING_SIDE, MOVE_CASTLE, sideToMove, ROOKS, KING, from, to);
                    
                } else {
                    
                    return createMove(0, BLACK_CASTLE_QUEEN_SIDE, MOVE_CASTLE, sideToMove, ROOKS, KING, from, to);
                }
            }
        }
    } else {
        
        ptrChar++;
        char fromFile = *ptrChar;
        
        ptrChar++;
        if(*ptrChar == (char)0) {
            return 0;
        }
        
        from = (int)*ptrChar - '0';
        from = ((from - 1) * 8) + addFileNumber(fromFile);
        
        ptrChar++;
        
        char toFile = *ptrChar;
        ptrChar++;
        to = (int)*ptrChar - '0';
        to = ((to - 1) * 8) + addFileNumber(toFile);
    }
    
    u64 toBitboard = getBitboardFromSquare(to);
    
    if(piece == PAWNS) {
        if (to == 1 || to == 8) {
            
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
            
            if(toBitboard & pieceBB[sideToMove ^ 1][PIECES]) {
            
                 if(toBitboard & pieceBB[sideToMove ^ 1][KNIGHTS]) {
                    
                    return createMove(promType, 0, MOVE_PROMOTION, sideToMove, KNIGHTS, piece, from, to);
                } else if(toBitboard & pieceBB[sideToMove ^ 1][BISHOPS]) {
                    
                    return createMove(promType, 0, MOVE_PROMOTION, sideToMove, BISHOPS, piece, from, to);
                } else if(toBitboard & pieceBB[sideToMove ^ 1][ROOKS]) {
                    
                    return createMove(promType, 0, MOVE_PROMOTION, sideToMove, ROOKS, piece, from, to);
                } else if(toBitboard & pieceBB[sideToMove ^ 1][QUEEN]) {
                    
                    return createMove(promType, 0, MOVE_PROMOTION, sideToMove, QUEEN, piece, from, to);
                }
            } else {
                
                return createMove(promType, 0, MOVE_PROMOTION, sideToMove, DUMMY, PAWNS, from, to);
            }
        } else if(sideToMove? from - 16 == to : from + 16 == to) {
            return createMove(0, 0, MOVE_DOUBLE_PUSH, sideToMove, DUMMY, PAWNS, from, to);
        } else if(toBitboard & pieceBB[sideToMove ^ 1][PIECES]) {
            
            if(toBitboard & pieceBB[sideToMove ^ 1][PAWNS]) {
                
                return createMove(0, 0, MOVE_CAPTURE, sideToMove, PAWNS, PAWNS, from, to);
            } else if(toBitboard & pieceBB[sideToMove ^ 1][KNIGHTS]) {
                
                return createMove(0, 0, MOVE_CAPTURE, sideToMove, KNIGHTS, PAWNS, from, to);
            } else if(toBitboard & pieceBB[sideToMove ^ 1][BISHOPS]) {
                
                return createMove(0, 0, MOVE_CAPTURE, sideToMove, BISHOPS, PAWNS, from, to);
            } else if(toBitboard & pieceBB[sideToMove ^ 1][ROOKS]) {
                
                return createMove(0, 0, MOVE_CAPTURE, sideToMove, ROOKS, PAWNS, from, to);
            } else if(toBitboard & pieceBB[sideToMove ^ 1][QUEEN]) {
                
                return createMove(0, 0, MOVE_CAPTURE, sideToMove, QUEEN, PAWNS, from, to);
            }
        } else {
            return createMove(0, 0, MOVE_NORMAL, sideToMove, DUMMY, PAWNS, from, to);
        }
    } else {
    
        if(toBitboard & occupied) {
            
            if(toBitboard & pieceBB[sideToMove ^ 1][PAWNS]) {
                
                return createMove(0, 0, MOVE_CAPTURE, sideToMove, PAWNS, piece, from, to);
            } else if(toBitboard & pieceBB[sideToMove ^ 1][KNIGHTS]) {
                
                return createMove(0, 0, MOVE_CAPTURE, sideToMove, KNIGHTS, piece, from, to);
            } else if(toBitboard & pieceBB[sideToMove ^ 1][BISHOPS]) {
                
                return createMove(0, 0, MOVE_CAPTURE, sideToMove, BISHOPS, piece, from, to);
            } else if(toBitboard & pieceBB[sideToMove ^ 1][ROOKS]) {
                
                return createMove(0, 0, MOVE_CAPTURE, sideToMove, ROOKS, piece, from, to);
            } else if(toBitboard & pieceBB[sideToMove ^ 1][QUEEN]) {
                
                return createMove(0, 0, MOVE_CAPTURE, sideToMove, QUEEN, piece, from, to);
            }
        } else {
        
            return createMove(0, 0, MOVE_NORMAL, sideToMove, DUMMY, piece, from, to);
        }
    }
    
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

    const u8 COLOR = sideToMove;
    u8 SIDE_TO_MOVE = COLOR;
    
    if(ptrChar != NULL) {

        ptrChar += 6;
        while(*ptrChar) {
            // parse Move
            
            u32 move = parseMove(ptrChar, SIDE_TO_MOVE);
            
            if(move == 0) {
                break;
            }

            make_move(move);
            ply = 0;
            
            while(*ptrChar && *ptrChar != ' ') {
                ptrChar++;
            }
            
            ptrChar++;
            SIDE_TO_MOVE ^= 1;
        }
    }

    print_board(occupied);
}

void parseGo(char *lineIn) {
    
}

bool quit = false;
void UciLoop() {
    
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    
    char line[INPUTBUFFER];
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
            parsePosition(line);
        } else if(!strncmp(line, "ucinewgame", 10)) {
            parsePosition("position startpos\n");
        } else if(!strncmp(line, "go", 2)) {
            parseGo(line);
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
