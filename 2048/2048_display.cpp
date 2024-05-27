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
        void show_tile(Pair pos, int v);
         void init_board_noref(); // same as init_board, but doesn't refresh


        
        Pair get_start_xy() {
            int i, j;

            getmaxyx(stdscr, i, j); // returns the x,y of the window's max corner

            int upadj = i/2 - board_height/2;
            int ladj = j/2 - board_width/2;

            return Pair(upadj, ladj);
        }



        void show_tile(Pair pos, int v) {
            if(v == 0) return;

            Pair start = get_start_xy();

            start.i += first_tile_row + tile_height * pos.i;
            start.j += first_tile_col + (tile_width + 2) * pos.j;


            for(int i = 0; i < tile_height; i++) {

                move(start.i + i, start.j + 1); // add 1 to col for left adjusted spacing
                addstr(empty_tile[i].c_str());
            }

            if(v > 0 && v < 10) move(start.i + 1, start.j + 2 + 2);
            else if(v >= 10 && v < 1000) move(start.i + 1, start.j + 2 + 1);
            else if(v >= 1000 && v < 10000) move(start.i + 1, start.j + 2);
            else return;

            addstr(std::to_string(v).c_str());

        }



        void init_board_noref() {
            Pair start = get_start_xy();

            // print the empty game board in the center of the window
            for(int i = 0; i < board_height; i++) {
                move(i + start.i, start.j);
                addstr(empty_board[i].c_str());
                addch('\n');
            }
        }
    }



    void init_board() {
        init_board_noref();
        refresh();
    }



    void show_board(game_state& state) {
        
        init_board_noref();

        Pair pos;

        for(int i = 0; i < state.size(); i++) {
            for(int j = 0; j < state.size(); j++) {
                pos.i = i;
                pos.j = j;
                show_tile(pos, state.board[i][j]);
            }
        }
        refresh();
    }



    void show_popup(std::string text) {
        
        Pair start = get_start_xy();

        start.i += popup_row;
        start.j += popup_col;


        for(int i = 0; i < popup_height; i++) {

            move(start.i + i, start.j); // add 1 to col for left adjusted spacing
            addstr(empty_popup[i].c_str());
        }

        move(start.i + 1, start.j + 1);
        addstr(text.c_str());

        refresh();
    }
}