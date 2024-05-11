#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <sstream>
#include <vector>

#include <curses.h>


using namespace std;

void rotating_underline(string s, int n_cycles, int cycle_time, pair<int, int> pos);

int main() {

    string hello = "Hello there!\n";

    initscr();

    printw(hello.c_str());
    pair<int, int> rotating_pos = {1, 0};
    rotating_underline("This sentence has a rotating underline!", 10, 200, rotating_pos);


    endwin();
    return 0;
}



void rotating_underline(const string s, const int n_cycles, const int cycle_time, pair<int, int> pos) {

    stringstream ss(s);
    vector<string> v;
    string word;


    while(ss >> word) v.push_back(word);

    // set cursor invisible
    if(curs_set(0) == ERR) beep();

    for(int n = 0; n < n_cycles; n++) { // runs the given number of cycles
        for(int i = 0; i < v.size(); i++) { // prints each sub-cycle (word underline)
            for(int j = 0; j < v.size(); j++) { // prints each word
                
                if(i == j) {
                    attron(A_UNDERLINE);
                    addstr(v[j].c_str());
                    attroff(A_UNDERLINE);
                }
                else {
                    addstr(v[j].c_str());
                }
                addch(' ');
            }
            move(pos.first, pos.second);
            refresh();
            napms(cycle_time);
        }
    }
}