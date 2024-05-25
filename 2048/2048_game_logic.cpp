#include <iostream>
#include <vector>
#include <random>

#include "2048_game_logic.h"

namespace game {

    namespace {

        int check_win(game_state& state); // look for win or lose conditions
        void update_state(int dir, game_state& state); // wrapper for merge
        bool merge(int dir, game_state& state);
        bool merge_in_line(int dir, game_state& state, int iter, int offset);

        // basic range function, inclusive lower bound, exclusive upper bound
        inline bool in(int v, int min, int max) {return v >= min && v < max;}



        void update_state(int dir, game_state& state) {

            state.changed = merge(dir, state); // make move

            if(!state.changed) return; // no change occured, do nothing

            state.win = check_win(state); // check for a win or loss
            if(state.win != NONE) return;

            state.add_tile(); // try to add a tile
            state.win = check_win(state); // check for a win or loss

            return;
        }



        bool merge(int dir, game_state& state) {
            
            bool if_change = false;
            
            for(int i = 0; i < state.size(); i++) {
                if_change |= merge_in_line(dir, state, i, 0);
            }

            return if_change;
        }



        bool merge_in_line(int dir, game_state& state, int iter, int offset) {

            // get useful values
            auto& board = state.board;
            auto start = state.start_pair(dir);
            auto update = state.update_pair(dir);
            auto look = state.look_pair(dir);

            // init search position
            int row = iter * update.i + start.i + offset * look.i;
            int col = iter * update.j + start.j + offset * look.j;

            if(!in(row, 0, board.size()) || !in(row, 0, board.size())) return false;

            int look_row, look_col;
            if(board[row][col] == 0) { // look and move

                look_row = row + look.i;
                look_col = col + look.j;
                while(in(look_row, 0, board.size()) && in(look_col, 0, board.size())) { // look-ahead

                    if(board[look_row][look_col] != 0) { // move
                        board[row][col] = board[look_row][look_col];
                        board[look_row][look_col] = 0;
                        merge_in_line(dir, state, iter, offset); // recursively call merge_in_line
                        return true;
                    }
                    look_row += look.i;
                    look_col += look.j;
                }
                return false;
            }

            // if its not 0, then we look for a possible merge
            
            look_row = row + look.i;
            look_col = col + look.j;
            while(in(look_row, 0, board.size()) && in(look_col, 0, board.size())) { // look-ahead

                if(board[look_row][look_col] == board[row][col]) { // matches value -> merge
                    board[row][col] *= 2;
                    board[look_row][look_col] = 0;
                    merge_in_line(dir, state, iter, 0); // recursively call merge_in_line
                    return true;
                }
                else if(board[look_row][look_col] != 0) { // doesn't match value, but is non-zero -> move or do nothing
                    return merge_in_line(dir, state, iter, offset + 1);
                }
                look_row += look.i;
                look_col += look.j;
            }

            return false;
        }



        int check_win(game_state& state) {

            auto& board = state.board;
            int zeros = 0;

            // search for the 2048 tile and count how many zeros are seen
            for(auto r: board) {
                for(auto n: r) {
                    if(n == 16) return WIN;
                    else if(n == 0) zeros++;
                }
            }
            // if zeros exist, then at least one row or column can be shifted
            if(zeros > 0) return NONE;
            
            // check for any possible merges in any direction
            for(std::size_t i = 0; i < board.size(); i++) {
                for(std::size_t j = 0; j < board.size(); j++) {
                    if(     in(i - 1, 0, board.size()) && in(j,     0, board.size()) && board[i][j] == board[i - 1][j    ]) return NONE;
                    else if(in(i + 1, 0, board.size()) && in(j,     0, board.size()) && board[i][j] == board[i + 1][j    ]) return NONE;
                    else if(in(i,     0, board.size()) && in(j - 1, 0, board.size()) && board[i][j] == board[i    ][j - 1]) return NONE;
                    else if(in(i,     0, board.size()) && in(j + 1, 0, board.size()) && board[i][j] == board[i    ][j + 1]) return NONE;
                }
            }

            // no possible merges found, return lose value
            return LOSE;
        }
    }



    void move(int dir, game_state& state) {

        if(dir != UP && dir != DOWN && dir != LEFT && dir != RIGHT) {
            std::cerr << "Invalid directional value given: `"<< (char)dir <<"`" << std::endl;
            state.changed = false;
            return;
        }
        
        update_state(dir, state);
    }
}
