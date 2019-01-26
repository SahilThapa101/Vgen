//
//  uci.h
//  Vgen
//
//  Created by Amar Thapa on 25/12/18.
//  Copyright © 2018 Amar Thapa. All rights reserved.
//

#ifndef uci_h
#define uci_h

#include <stdio.h>
#include "globals.h"

void UciLoop(void);
void parsePosition(char* lineIn);
u32 parseMove(char *ptrChar, u8 color);
int addFileNumber(char ch);

#endif /* uci_h */
