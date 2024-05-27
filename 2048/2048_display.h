#ifndef _2048_DISPLAY_H
#define _2048_DISPLAY_H

#include <vector>
#include <string>

#include "2048_game_state.h"

// you must be in a curses context already for these functions to work!!!

namespace display {

    void init_board();
    void show_board(game_state& state);
    void show_popup(std::string text);

    
    const std::vector<std::string> empty_board {
        "       ┏━━━┳━━━┳━┳━┳━━━┓       ",
        "       ┣━  ┃ ╻ ┃ ╹ ┃ ╹ ┃       ",
        "       ┃  ━┫ ╹ ┣━┓ ┃ ╹ ┃       ",
        "┏━━━━━━┻━━━┻━━━┻━┻━┻━━━┻━━━━━━┓",
        "┃                             ┃",
        "┃                             ┃",
        "┃                             ┃",
        "┃                             ┃",
        "┃                             ┃",
        "┃                             ┃",
        "┃                             ┃",
        "┃                             ┃",
        "┃                             ┃",
        "┃                             ┃",
        "┃                             ┃",
        "┃                             ┃",
        "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛",
        " 'w', 'a', 's', or 'd' to Move ",
        "  'q' to Quit, 'r' to Restart  ",
        "                               ",
        "          WRITTEN BY           ",
        "         ETHAN WOOLLET         ",
    };

    const int board_height = empty_board.size();
    const int board_width = empty_board.back().size(); // THE REST OF THE STRINGS ARE UNICODE AAHHHH


    const std::vector<std::string> empty_tile {
        "┌────┐",
        "│    │",
        "└────┘"
    };

    const int tile_height = empty_tile.size();
    const int tile_width = (empty_tile[0].size() - 1) / 3; // these unicode chars are considered 3 ascii chars, I think

    const int first_tile_row = 4;
    const int first_tile_col = 1;


    const std::vector<std::string> empty_popup = {
        "┏━━━━━━━━━┓",
        "┃         ┃",
        "┗━━━━━━━━━┛"
    };

    const int popup_height = empty_popup.size();
    const int popup_width = (empty_popup[0].size() - 1) / 3; // these unicode chars are considered 3 ascii chars, I think

    const int popup_row = 8;
    const int popup_col = 10;
}

#endif