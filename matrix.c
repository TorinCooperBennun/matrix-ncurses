/*
    Copyright (C) 2017 Torin Cooper-Bennun

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/

#include "matrix.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>


// in milliseconds
#define  TICK_DURATION        100
#define  MAX_STREAMS          10
// in ticks per new stream
#define  NEW_STREAM_GEN_RATE  5


int      max_y;
int      max_x;

int      tick_count = 0;


typedef struct {
    // top-most coord of the stream
    int  y;
    int  x;
    // current length
    int  len;
    // speed at which len grows (ticks per new character)
    int  speed;
    int  birth_tick;
}
m_stream;

m_stream streams[MAX_STREAMS];
int      active_streams = 0;


void PlaceRandomChar(int y, int x) {
    char start = '!';
    char end = '~';
    char c = (rand() % (end - start + 1)) + start;
    mvaddch(y, x, c);
}


void UpdateStreams(void) {
    int i;
    int y;
    int x;
    m_stream *s = streams;
    for (i = 0; i < active_streams; ++i, ++s) {
        if (s->birth_tick == tick_count) {
            for (y = 0; y < s->len; ++y) {
                PlaceRandomChar(s->y + y, s->x);
            }
        }
    }
}


void InitCurses(void) {
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, true);

    getmaxyx(stdscr, max_y, max_x);
}


void ExitCurses(void) {
    endwin();
}


void TickSleep(void) {
    static const struct timespec req = {0, TICK_DURATION * 1000000};
    nanosleep(&req, NULL);
}


void Matrix(void) {
    m_stream stream;

    InitCurses();

    for (;;) {
        if (getch() != ERR) break;

        if (active_streams < MAX_STREAMS) {
            if (tick_count % NEW_STREAM_GEN_RATE == 0) {
                stream.y          = rand() % (max_y / 4 * 3);
                stream.x          = rand() % (max_x + 1);
                stream.len        = 1 + (rand() % 5);
                stream.birth_tick = tick_count;
                stream.speed      = 1 + (rand() % 4);
                streams[active_streams++] = stream;
            }
        }
        UpdateStreams();

        ++tick_count;
        TickSleep();
    }

    ExitCurses();
    printf("%d ticks\n", tick_count);
}
