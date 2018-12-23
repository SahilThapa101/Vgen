//
//  search.c
//  Vgen
//
//  Created by Amar Thapa on 23/12/18.
//  Copyright © 2018 Amar Thapa. All rights reserved.
//

#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "search.h"
#include "evaluate.h"
#include "globals.h"
#include "movegen.h"
#include "make_unmake.h"
#include "utility.h"

#ifndef piece_name
char pieceName[2][8] = { { ' ', (char) 0, 'N', 'B', 'R', 'Q', 'K', '\0'}, { ' ', (char) 0, 'n', 'b', 'r', 'q', 'k', '\0'}};
#endif

u64 getZobristKeyForPosition() {
    
    u8 piece = KING;
    u64 key = 0;
    u64 bitboard;
    for(int i = 1; i <= piece; i++) {
        for (int j = 0; j < 2; j++) {
            
            bitboard = pieceBB[j][i];
            while(bitboard) {
                // j = color, piece = piece type, bitScanForward(bitboard) returns square of LSB
                key ^= zobrist[j][piece][bitScanForward(bitboard)];
                bitboard &= bitboard - 1;
            }
        }
    }
    
    return key;
}

void search(u8 color) {
    
    clock_t start, end;
    double cpu_time_used;
    const int MATE = 5000;
    u32 move;
    const u8 COLOR = color;
    LINE mainline;
    
    start = clock();
    for(int i = 1; ; i++) {
        ply = 0;
        
        int score = alphabeta(COLOR, i, -10000, 10000, MATE, &mainline);
        
        // if(score >= MATE - 1000) {// Handle up to mate in 500 or so.
        
        //     printf("Mate in %d\n", (MATE - score + 1) / 2);
        // } else if(score <= -MATE + 1000) {
        
        //     printf("Mated in %d\n", (MATE + score) / 2);
        // }
        
        end = clock();
        
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        
        int numberOfMoves = mainline.cmove;
        
        printf("\n%d) ", i);
        for (int j = 0; j < numberOfMoves; j++) {
            
            move = mainline.argmove[j];
            
            printf("%c%s-%s ", pieceName[colorType(move)][pieceType(move)],
                   algebricPos(from_sq(move)), algebricPos(to_sq(move)));
        }
        
        printf(" %d %fs", score, cpu_time_used);
    }
}

int alphabeta(int color, int depth, int alpha, int beta, int mate, LINE *pline) {
    
    int hashf = hashfALPHA;
    int value = ProbeHash(color, depth, alpha, beta);
    if(value != VAL_UNKNOWN) {
        
        return value;
    }
    
    if (depth == 0) {
        
        pline->cmove = 0;
        
        int score = evaluate(color);
        
        RecordHash(color, depth, score, hashfEXACT);
        
        return score;
    }
    
    ply = ply + 1;
    moveStack[ply].epFlag = moveStack[ply - 1].epFlag;
    moveStack[ply].epSquare = moveStack[ply - 1].epSquare;
    moveStack[ply].castleFlags  =  moveStack[ply - 1].castleFlags;
    
    LINE line;
    
    u32 numberOfMoves;
    u32 moveList[MAX_MOVES];
    
    numberOfMoves = genMoves(moveList, color);
    
    int legalMoves = 0;
    
    for (int i = 0; i < numberOfMoves; i++) {
        
        make_move(moveList[i]);
        
        if (!isKingInCheck(color)) {
            
            int val = -alphabeta(color ^ 1, depth - 1, -beta, -alpha, mate - 1, &line);
            
            unmake_move(moveList[i]);
            
            if (val >= beta) {
                
                RecordHash(color, depth, beta, hashfBETA);
                
                ply = ply - 1;
                return beta;
            }
            
            if (val > alpha) {
                
                hashf = hashfEXACT;
                
                alpha = val;
                
                pline->argmove[0] = moveList[i];
                memcpy(pline->argmove + 1, line.argmove, line.cmove * sizeof(u32));
                pline->cmove = line.cmove + 1;
            }
            
            legalMoves++;
            
            continue;
        }
        
        unmake_move(moveList[i]);
    }
    
    ply = ply - 1;
    
    if(legalMoves == 0) {
        if (!isKingInCheck(color)) {
            // stalemate
            float score = color ? 0.25 : -0.25;
            RecordHash(color, depth, 0, hashf);
            return score;
        } else {
            // checkmate
            
            int score = color ? mate : -mate;
            RecordHash(color, depth, score, hashf);
            return score;
        }
    }
    
    RecordHash(color, depth, 0, hashf);
    return alpha;
}

int Quies(int color, int alpha, int beta) {
    
    int val = evaluate(color);
    
    if (val >= beta) {
        return beta;
    }
    
    if (val > alpha) {
        alpha = val;
    }
    
    ply = ply + 1;
    moveStack[ply].epFlag = moveStack[ply - 1].epFlag;
    moveStack[ply].epSquare = moveStack[ply - 1].epSquare;
    moveStack[ply].castleFlags  =  moveStack[ply - 1].castleFlags;
    
    u32 numberOfCaptures;
    u32 moveList[MAX_MOVES];
    
    numberOfCaptures = genCaptures(moveList, color);
    
    // order by MVV-LVA
    // PxQ, B/NXQ, RXQ, QXQ, KXQ - Queen Captures
    
    // PXR, B/NXR, RXR, QXR, KXR - Rook Captures
    
    // PXB/N, B/NXB/N, RXB/N, KXB/N - Bishop/Knight Captures
    
    // PXP, B/NXP, RXP, QXP, KXP - Pawn Captures
    
    if(numberOfCaptures > 1) {
        MVV_LVA(moveList, numberOfCaptures);
    }

    for (int i = 0; i < numberOfCaptures; i++) {
        
        make_move(moveList[i]);
        
        if (!isKingInCheck(color)) {
            
            val = -Quies(color ^ 1, -beta, -alpha);
            
            unmake_move(moveList[i]);
            
            if (val >= beta) {
                
                ply = ply - 1;
                return beta;
            }
            if (val > alpha) {
                alpha = val;
            }
            
            continue;
        }
        
        unmake_move(moveList[i]);
    }
    
    ply = ply - 1;
    
    return alpha;
}

void MVV_LVA(u32 *moveList, u32 numberOfMoves) {
    
    // int queenCapturesCounter = 0;
    // int rookCapturesCounter = 0;
    // int bishopCapturesCounter = 0;
    // int knightCapturesCounter = 0;
    // int pawnCapturesCounter = 0;
    // int capturesCounter = 0;
    
    // u32 queenCaptures[MAX_MOVES];
    // u32 rookCaptures[MAX_MOVES];
    // u32 bishopCaptures[MAX_MOVES];
    // u32 knightCaptures[MAX_MOVES];
    // u32 pawnCaptures[MAX_MOVES];
    
//    for(int k = 0; k < numberOfMoves; k++) {
//        u32 move = moveList[k];
//        printf("%c%s-%s ", pieceName[colorType(move)][pieceType(move)],
//               algebricPos(from_sq(move)), algebricPos(to_sq(move)));
//    }
//
    u8 cPiece;
    u8 piece;
    int score[MAX_MOVES];
    for(int k = 0; k < numberOfMoves; k++) {
        
        cPiece = cPieceType(moveList[k]);
        piece = pieceType(moveList[k]);
        
        score[k] = 64 * cPiece - piece;
    }
    
    u8 tempScore;
    u32 tempMove;
    for (int i = 0; i < numberOfMoves; i++) {
        for (int j = i + 1; j < numberOfMoves; j++) {
            
            if (score[i] < score[j]) {/* For decreasing order use < */
                tempMove = moveList[i];
                moveList[i] = moveList[j];
                moveList[j] = tempMove;
                
                tempScore = score[i];
                score[i] = score[j];
                score[j] = tempScore;
            }
        }
    }
    
//    for (i = 1; i < numberOfMoves; i++) {
//        move = moveList[i];
//        j = i - 1;
//
//        piece1 = cPieceType(moveList[j]);
//        piece2 = pieceType(moveList[j]);
//
//        score1 = 64 * piece1 - piece2;
//
//        piece1 = cPieceType(move);
//        piece2 = pieceType(move);
//
//        score2 = 64 * piece1 - piece2;
//
//        while (j >= 0 && score1 < score2) {
//            moveList[j + 1] = moveList[j];
//            score[j + 1] = score1;
//            j = j - 1;
//        }
//
//        moveList[j + 1] = move;
//        score[j + 1] = score2;
//    }
    
//    printf("\n");
//    for(int l = 0; l < numberOfMoves; l++) {
//        u32 move = moveList[l];
//        printf("%c%s-%s(%d) ", pieceName[colorType(move)][pieceType(move)],
//               algebricPos(from_sq(move)), algebricPos(to_sq(move)), score[l]);
//    }

//    printf("\n");
//    printf("\n");

    
    // for(int i = 0; i < numberOfMoves; i++) {
    
    //     move = moveList[i];
    
    //     switch(cPieceType(move)) {
    //         case QUEEN:
    //             queenCaptures[queenCapturesCounter++] = move;
    //             capturesCounter = capturesCounter + 1;
    //             break;
    //         case ROOKS:
    //             rookCaptures[rookCapturesCounter++] = move;
    //             capturesCounter = capturesCounter + 1;
    //             break;
    //         case BISHOPS:
    //             bishopCaptures[bishopCapturesCounter++] = move;
    //             capturesCounter = capturesCounter + 1;
    //             break;
    //         case KNIGHTS:
    //             knightCaptures[knightCapturesCounter++] = move;
    //             capturesCounter = capturesCounter + 1;
    //             break;
    //         case PAWNS:
    //             pawnCaptures[pawnCapturesCounter++] = move;
    //             capturesCounter = capturesCounter + 1;
    //             break;
    //         default:
    //             break;
    //     }
    // }
    
    // if(queenCapturesCounter) {
    //     sortByLVA(queenCaptures, queenCapturesCounter);
    // }
    // if(rookCapturesCounter) {
    //     sortByLVA(rookCaptures, rookCapturesCounter);
    // }
    // if(bishopCapturesCounter) {
    //     sortByLVA(bishopCaptures, bishopCapturesCounter);
    // }
    // if(knightCapturesCounter) {
    //     sortByLVA(knightCaptures, knightCapturesCounter);
    // }
    // if(pawnCapturesCounter) {
    //     sortByLVA(pawnCaptures, pawnCapturesCounter);
    // }
    
    // u32 capturesSortedByMVA_LVA[MAX_MOVES];
    
    // int count = 0;
    // int loopCount = 0;
    // while(queenCapturesCounter > 0) {
    
    //     capturesSortedByMVA_LVA[count] = queenCaptures[loopCount];
    //     queenCapturesCounter--;
    //     loopCount++;
    //     count++;
    // }
    
    // loopCount = 0;
    // while(rookCapturesCounter > 0) {
    
    //     capturesSortedByMVA_LVA[count] = rookCaptures[loopCount];
    //     rookCapturesCounter--;
    //     loopCount++;
    //     count++;
    // }
    
    // loopCount = 0;
    // while(bishopCapturesCounter > 0) {
    
    //     capturesSortedByMVA_LVA[count] = bishopCaptures[loopCount];
    //     bishopCapturesCounter--;
    //     loopCount++;
    //     count++;
    // }
    
    // loopCount = 0;
    // while(knightCapturesCounter > 0) {
    
    //     capturesSortedByMVA_LVA[count] = knightCaptures[loopCount];
    //     knightCapturesCounter--;
    //     loopCount++;
    //     count++;
    // }
    
    // loopCount = 0;
    // while(pawnCapturesCounter > 0) {
    
    //     capturesSortedByMVA_LVA[count] = pawnCaptures[loopCount];
    //     pawnCapturesCounter--;
    //     loopCount++;
    //     count++;
    // }
    
    // moveList = &capturesSortedByMVA_LVA;
    
    // return capturesCounter;
}

int ProbeHash(int color, int depth, int alpha, int beta) {
    
    u64 key = getZobristKeyForPosition();
    
    if(color) {
        key ^= KEY_BLACK_TO_MOVE;
    }
    
    HASHE *phashe = &hashTable[key % HASH_TABLE_SIZE];
    
    if (phashe->key == key) {
        
        if (phashe->depth >= depth) {
            
            if (phashe->flags == hashfEXACT) {
                return phashe->value;
            }
            
            if ((phashe->flags == hashfALPHA) && (phashe->value <= alpha)) {
                return alpha;
            }
            
            if ((phashe->flags == hashfBETA) && (phashe->value >= beta)) {
                return beta;
            }
        }
        
        // RememberBestMove();
    }
    
    return VAL_UNKNOWN;
}

void RecordHash(int color, int depth, int val, int hashf) {
    
    u64 key = getZobristKeyForPosition();
    
    if(color) {
        key ^= KEY_BLACK_TO_MOVE;
    }
    
    HASHE * phashe = &hashTable[key % HASH_TABLE_SIZE];
    
    phashe->key = key;
    // phashe->best = BestMove();
    phashe->value = val;
    phashe->flags = hashf;
    phashe->depth = depth;
}


// void sortByLVA(u32 *moveList, int numberOfMoves) {

//     for(int i = numberOfMoves - 1; i >= 0; i--) {
//          for (int j = 1; j <= i; j++)    {
//              if (pieceType(moveList[j-1]) < pieceType(moveList[j])) {
//                 u32 temp = moveList[j-1];
//                 moveList[j-1] = moveList[j];
//                 moveList[j] = temp;
//                }
//            }
//        }
// }

// void sortByLVA(u32 *moveList, int numberOfMoves) {

//     int i;
//     int j;
//     u32 move;
//     for (i = 1; i < numberOfMoves; i++) {
//         move = moveList[i];
//         j = i - 1;

//         while (j >= 0 && pieceType(moveList[j]) < pieceType(move)) {
//             moveList[j + 1] = moveList[j];
//             j = j - 1;
//         }

//         moveList[j + 1] = move;
//     }
// }

// void insertionSort(int arr[], int n) {
//    int i, key, j;
//    for (i = 1; i < n; i++) {
//        key = arr[i];
//        j = i-1;

//         Move elements of arr[0..i-1], that are
//           greater than key, to one position ahead
//           of their current position
//        while (j >= 0 && arr[j] > key) {
//            arr[j+1] = arr[j];
//            j = j-1;
//        }

//        arr[j+1] = key;
//    }
// }










