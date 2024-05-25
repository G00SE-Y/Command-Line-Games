#ifndef _2048_GAME_LOGIC_H
#define _2048_GAME_LOGIC_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "2048_game_state.h"

namespace game {
    
    // Directly modifies the given board and returns a valid game_return structure
    void move(int dir, game_state& state);
}

#endif