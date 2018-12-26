//
//  vtime.c
//  Vgen
//
//  Created by Amar Thapa on 26/12/18.
//  Copyright © 2018 Amar Thapa. All rights reserved.
//

#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>

#include "vtime.h"

u64 getTimeMs() {
    
    struct timeval t;
    gettimeofday(&t, NULL);
    
    return (int)(t.tv_sec * 1000) + (t.tv_usec / 1000);
}

int inputWaiting() {
    
#ifndef WIN32
    fd_set readfds;
    struct timeval tv;
    
    FD_ZERO (&readfds);
    FD_SET (fileno(stdin), &readfds);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    select(16, &readfds, 0, 0, &tv);
    
    return (FD_ISSET(fileno(stdin), &readfds));
    
#else
    static int init = 0, pipe;
    static HANDLE inh;
    DWORD dw;
    
    if(!init) {
        init = 1;
        inh = GetStdHandle(STD_INPUT_HANDLE);
        pipe = !GetConsoleMode(inh, &dw);
        if(!pipe) {
            SetConsoleMode(inh, dw & ~(ENABLE_MOUSE_INPUT|ENABLE_WINDOW_INPUT));
            FlushConsoleInputBuffer(inh);
        }
    }
    
    if(pipe) {
        if(!PeekNamedPipe(inh, NULL, 0, NULL, &dw, NULL)) {
            return 1;
        }
        
        return dw;
    } else {
        GetNumberOfConsoleInputEvents(inh, &dw);
        return dw <= 1 ? 0 : dw;
    }
    
#endif
}

void readInput() {
    
    int bytes;
    char input[256] = "";
    char *endc;
    
    if(inputWaiting()) {
        stopped = true;
        do {
            bytes = (int)read(fileno(stdin), input, 256);
        } while(bytes < 0);
        
        endc = strchr(input, '\n');
        if(endc) {
            *endc = 0;
        }
        
        if(strlen(input) > 0) {
            if(!strncmp(input, "quit", 4)) {
                quit = true;
            }
        }
    }
}