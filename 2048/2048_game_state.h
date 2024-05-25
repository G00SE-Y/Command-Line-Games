#ifndef _2048_GAME_STATE_H
#define _2048_GAME_STATE_H

#include <vector>
#include <random>
#include <unordered_map>
#include <string>

#include "2048_utils.h"

// contains all you need to play the game
class game_state {

    public:

        bool changed; // true if the given move resulted in the board changing, else false
        int win; // 1 if game met the win condition, -1 if loss condition, 0 otherwise
        int len; // a board is a square of side length `size`
        std::vector<std::vector<int>> board; // a reference to the game board

        bool add_tile();
        std::string get_string_board();

        // constructor
        game_state(int n);

        // getters
        int size();
        Pair start_pair(int dir);
        Pair update_pair(int dir);
        Pair look_pair(int dir);

    private:

        // structure used by the merging algorithm
        typedef struct alg_struct{
            pair start;     // what index to begin at
            pair update;    // what direction is the next line (i loop)
            pair look;      // what direction to look-ahead (j loop)
            alg_struct(){};
            alg_struct(pair p1, pair p2, pair p3) {start = p1, update = p2, look = p3;};
        }alg;

        std::mt19937 gen; // rng machine
        std::unordered_map<int, alg_struct> alg_info; // very important hardcoded stuff for the merging algorithm
};

#endif