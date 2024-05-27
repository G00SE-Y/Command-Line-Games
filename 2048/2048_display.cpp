#include <vector>
#include <string>
#include <iostream>

#include <curses.h>

#include "2048_display.h"
#include "2048_game_state.h"
#include "2048_utils.h"

namespace display {

    namespace {
        Pair get_start_xy();
        void show_empty_board();

        Pair get_start_xy() {
            int x, y;

            getmaxyx(stdscr, y, x);

            int ladj = x/2 - empty_board[0].size()/2;
            int upadj = y/2 - empty_board.size()/2;

            std::cout << "there are " << x << " columns and " << y << " rows in this window" << std::endl;
            std::cout << "2048 would print starting at " << ladj << " columns and " << upadj << " rows" << std::endl;
            return Pair(x - ladj, y - upadj);
        }

        void show_empty_board() {
            Pair start = get_start_xy();

            for(std::size_t i = 0; i < empty_board.size(); i++) {
                move(i + start.i, start.j);
                printw(empty_board[i].c_str());
                addch('\n');
            }

            refresh();
            getch();
        }
    }



    void show_board(game_state& state) {
        show_empty_board();
    }
}