//
//  fen.h
//  Vgen
//
//  Created by Amar Thapa on 25/12/18.
//  Copyright © 2018 Amar Thapa. All rights reserved.
//

#ifndef fen_h
#define fen_h

#include <stdio.h>
#include "globals.h"

typedef void (*split_fn)(const char *, size_t, void *);
void initFEN(const char *str, size_t len, void *data);
void readFEN(char* FEN, int len);
void writeFen(char* str);
void fen(char* str);
void splitTheFEN(void);

#endif /* fen_h */
