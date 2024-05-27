#ifndef _2048_DISPLAY_H
#define _2048_DISPLAY_H

#include <vector>
#include <string>

#include "2048_game_state.h"

// you must be in a curses context already for these functions to work!!!

namespace display {
    void show_board(game_state& state);

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
        "",
        "          WRITTEN BY           ",
        "         ETHAN WOOLLET         ",
    };
}

#endif