#include <iostream>
#include <string>
#include <fcntl.h>

#include <curses.h>
#include <locale.h>
#include <Windows.h>

#include "2048_game_logic.h"
#include "2048_display.h"
#include "2048_game_state.h"
#include "2048_utils.h"

#define MIN_BOARD 2
#define MAX_BOARD 6


bool play_game(int n);


using std::cout, std::endl;

int main(int argv, char** args) {

    int board_size = 4;

    try {
        if(argv == 2) board_size = std::stoi(args[1]);

        if(board_size < MIN_BOARD || board_size > MAX_BOARD) {
            cout << "Please enter a board size between " << MIN_BOARD << " and " << MAX_BOARD << endl;
            return 0;
        }
    }
    catch (std::invalid_argument &e) {
        cout << "`" << args[1] << "` is not a valid number" << endl;
        return 0;
    }

    while(play_game(board_size));

    return 0;
}

bool play_game(int n) {
    auto state = game_state(n);
    
    initscr();
    
    auto old_code_page = GetConsoleOutputCP();
    SetConsoleOutputCP(CP_UTF8);

    std::cout << "Unicode character...?  " << "\u250F" << std::endl;

    noecho();
    curs_set(0);
    display::show_board(state);
    return false;
    addstr(state.get_string_board().c_str());
    move(0,0);
    refresh();

    char c;
    while(state.win == NONE) {
        c = getch();
        c = tolower(c); // get input

        if(c == QUIT) { // exit program
            endwin();
            return false;
        }
        else if(c == RESTART) { // restart game
            endwin();
            return true;
        }
        else if(c != LEFT && c != RIGHT && c != UP && c != DOWN) continue; // not a recognized direction
        
        game::move(c, state); // try to perform move

        if(!state.changed) continue; // no changes happened as a result of the direction given, try again

        addstr(state.get_string_board().c_str());
        move(0,0);
        refresh();
    }

    move(n + 2, 0);
    if(state.win == WIN) {
        addstr("You Win!");
    }
    else if(state.win == LOSE) {
        addstr("You Lose!");
    }
    else {
        addstr("You What?!");
    }
    addstr("\n\nPress any key to exit..");
    refresh();
    getch();

    SetConsoleOutputCP(old_code_page);

    endwin();

    return false;
}