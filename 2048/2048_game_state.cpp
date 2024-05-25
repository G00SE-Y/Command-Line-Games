#include <stdexcept>
#include <unordered_map>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#include "2048_utils.h"
#include "2048_game_state.h"

game_state::game_state(int n) {

    // rng stuff
    std::random_device rd;
    gen = std::mt19937(rd());
    
    // the magic numbers
    alg_info = std::unordered_map<int, alg_struct>({
        {UP, alg_struct(Pair(0, 0), Pair(0, 1), Pair(1, 0))},
        {DOWN, alg_struct(Pair(n - 1, 0), Pair(0, 1), Pair(-1, 0))},
        {LEFT, alg_struct(Pair(0, 0), Pair(1, 0), Pair(0, 1))},
        {RIGHT, alg_struct(Pair(0, n - 1), Pair(1, 0), Pair(0, -1))},
    });

    len = n;
    changed = false;
    win = NONE;

    // init board with all 0's
    board = std::vector<std::vector<int>>(n, std::vector<int>(n, 0));

    // add 2 random starting tiles
    add_tile();
    add_tile();
}



bool game_state::add_tile() {

    // list of empty spaces
    std::vector<Pair> positions;

    // find empty spaces
    for(int i = 0; i < len; i++) {
        for(int j = 0; j < len; j++) {

            if(board[i][j] == 0) positions.push_back(Pair(i,j));
        }
    }

    // no empty spaces, can't add a tile
    if(positions.size() == 0) return false;

    // add a new tile
    std::uniform_int_distribution<> dist(0, board.size() - 1);
    Pair p = positions.at(dist(gen));
    board[p.i][p.j] = 2;

    return true;
}



std::string game_state::get_string_board() {
    std::stringstream ss;
    
    for(auto r: board) {
        
        ss << "[";
        
        for(auto x: r) {

            ss << std::setw(6) << x;
        }
        ss << " ]\n";
    }

    return ss.str();
}



// getters
int game_state::size() { return len; }
Pair game_state::start_pair(int dir) { return alg_info.at(dir).start; }
Pair game_state::update_pair(int dir) { return alg_info.at(dir).update; }
Pair game_state::look_pair(int dir) { return alg_info.at(dir).look; }