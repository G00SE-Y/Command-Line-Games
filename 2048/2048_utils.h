#ifndef _2048_UTILS_H
#define _2048_UTILS_H

// keyboard input enumeration
enum directions{
    UP = 'w',
    DOWN = 's',
    LEFT = 'a',
    RIGHT = 'd',
    QUIT = 'q',
    RESTART = 'r'
};

// values representing a win loss or neither
enum win_values{
    WIN,
    LOSE,
    NONE
};


// simple int pair struct
typedef struct Pair{
    int i;
    int j;
    Pair(){};
    Pair(int v1, int v2) {i = v1, j = v2;};
}pair;

#endif