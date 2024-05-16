#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <unordered_map>
#include <cmath>

#include <curses.h>


#define MIN_BOARD 4
#define MAX_BOARD 20

typedef struct ScreenWrite {
    std::pair<int, int> pos;
    std::string str;
    
    ScreenWrite(){};
    ScreenWrite(int x, int y, std::string s) {
        pos = std::make_pair(x, y);
        str = s;
    };
}write;

enum {
    UP = 'w',
    DOWN = 's',
    LEFT = 'a',
    RIGHT = 'd',
    QUIT = 'q',
    RESTART = 'r'
};

bool play_game(int size);
void write_changes(std::vector<write> changes);
int check_win(std::vector<std::vector<int>> &board);
bool update_board(char dir, std::vector<std::vector<int>> &board);
bool merge(int dir, std::vector<std::vector<int>> &board);
bool merge_in_line(int dir, std::vector<std::vector<int>> &board, int iter, int offset);
void init_update_map(int n);
char get_input();

void print_board(std::vector<std::vector<int>> &board); // debug

// range function
inline bool in(int v, int min, int max) {return v >= min && v < max;}

using std::cout, std::endl;

int main(int argv, char** args) {

    int board_size = 4;

    try {
        if(argv == 2) board_size = std::stoi(args[1]);

        if(board_size < MIN_BOARD || board_size > MAX_BOARD) {
            cout << "Please enter a board size between 4 and 20" << endl;
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



bool play_game(int size) {

    init_update_map(size);

    /*
    Seting up random number generator
    I'm using uniform_int_distribution because I want every value to be equally likely
    */
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, size - 1);

    // init board with all 0's
    std::vector<std::vector<int>> board = std::vector<std::vector<int>>(size, std::vector<int>(size, 0));

    // add 2 random starting pieces of value 2
    int x, y;
    while(true) {

        x = dist(gen);
        y = dist(gen);
        if(board[x][y] == 0) {
            board[x][y] = 2;
            break;
        }
    }

    // game loop
    int is_win = 0; // +1 == win, -1 == lose
    char input;

    while(is_win == 0) {

        // generate new tile
        while(true) { // this can be improved, but it might require some system changes
            x = dist(gen);
            y = dist(gen);
            if(board[x][y] == 0) {
                board[x][y] = 2;
                break;
            }
        }
        
        print_board(board);
        while(true) { // input loop, do nothing if given invalid inputs

            cout << "Move: ";
            input = get_input();

            if(input == QUIT) {
                cout << "Quitting!" << endl;
                return false;
            }
            else if(input == RESTART) {
                cout << "Restarting!" << endl;
                return true;
            }
            else if (input == LEFT || input == RIGHT || input == UP || input == DOWN) {
                if(check_win(board) != 0) break;
                if(update_board(input, board)) break;
            }
        }
        is_win = check_win(board);
        if(is_win) break;
    }

    print_board(board);

    if(is_win > 0) {
        cout << "You win!" << endl;
    }
    else if (is_win < 0) {
        cout << "You lose!" << endl;
    }

    return false;
}


bool update_board(char dir, std::vector<std::vector<int>> &board) {

    if(dir != UP && dir != DOWN && dir != LEFT && dir != RIGHT) return false;
    return merge(dir, board);

}



typedef struct Pair{
    int i;
    int j;
    Pair(){};
    Pair(int v1, int v2) {i = v1, j = v2;};
}pair;

typedef struct alg_stuff{
    pair start;
    pair update;
    pair look;
    alg_stuff(){};
    alg_stuff(pair p1, pair p2, pair p3) {start = p1, update = p2, look = p3;};
}alg;


std::unordered_map<int, alg> alg_info; // global cuz I'm goofy
void init_update_map(int n) {

    alg_info = std::unordered_map<int, alg>({
        {UP, alg_stuff(Pair(0, 0), Pair(0, 1), Pair(1, 0))},
        {DOWN, alg_stuff(Pair(n - 1, 0), Pair(0, 1), Pair(-1, 0))},
        {LEFT, alg_stuff(Pair(0, 0), Pair(1, 0), Pair(0, 1))},
        {RIGHT, alg_stuff(Pair(0, n - 1), Pair(1, 0), Pair(0, -1))},
    });

    // for(auto [x, n]: alg_info) {
    //     cout << char(x) << " -> start(" << n.start.i << ", " << n.start.j << ")   update(" << n.update.i << ", " << n.update.j << ")   look(" << n.look.i << ", " << n.look.j << ")" << endl;
    // }
}

bool merge(int dir, std::vector<std::vector<int>> &board) {
    
    bool if_change = false;
    
    for(std::size_t i = 0; i < board.size(); i++) {
        if_change |= merge_in_line(dir, board, i, 0);
    }

    return if_change;
}



bool merge_in_line(int dir, std::vector<std::vector<int>> &board, int iter, int offset) {

    int row = iter * alg_info[dir].update.i + alg_info[dir].start.i + offset * alg_info[dir].look.i;
    int col = iter * alg_info[dir].update.j + alg_info[dir].start.j + offset * alg_info[dir].look.j;

    // cout << "start at(" << row << ", " << col << "), offset: " << offset << endl;
    // print_board(board);
    if(!in(row, 0, board.size()) || !in(row, 0, board.size())) return false;

    int look_row, look_col;
    if(board[row][col] == 0) { // look and move

        look_row = row + alg_info[dir].look.i;
        look_col = col + alg_info[dir].look.j;
        while(in(look_row, 0, board.size()) && in(look_col, 0, board.size())) { // look-ahead
            // cout << "looking to shift from (" << look_row << ", " << look_col << ") into (" << row << ", " << col << ")" << endl;
            if(board[look_row][look_col] != 0) { // move
                board[row][col] = board[look_row][look_col];
                board[look_row][look_col] = 0;
                // cout << "moved" << endl;
                merge_in_line(dir, board, iter, offset); // recursively call merge_in_line
                return true;
            }
            look_row += alg_info[dir].look.i;
            look_col += alg_info[dir].look.j;
        }
        return false;
    }

    // if its not 0, then we look for a possible merge
    
    look_row = row + alg_info[dir].look.i;
    look_col = col + alg_info[dir].look.j;
    while(in(look_row, 0, board.size()) && in(look_col, 0, board.size())) { // look-ahead
        // cout << "looking to merge from (" << look_row << ", " << look_col << ") into (" << row << ", " << col << ")" << endl;
        if(board[look_row][look_col] == board[row][col]) { // matches value -> merge
            board[row][col] *= 2;
            board[look_row][look_col] = 0;
            // cout << "merged" << endl;
            merge_in_line(dir, board, iter, 0); // recursively call merge_in_line
            return true;
        }
        else if(board[look_row][look_col] != 0) { // doesn't match value, but is non-zero -> move or do nothing
            return merge_in_line(dir, board, iter, offset + 1);
        }
        look_row += alg_info[dir].look.i;
        look_col += alg_info[dir].look.j;
    }

    return false;
}



void write_changes(std::vector<write> changes){

}




int check_win(std::vector<std::vector<int>> &board) {
    int zeros = 0;

    for(auto l: board) {
        for(auto n: l) {
            if(n == 2048) return 1;
            else if(n == 0) zeros++;
        }
    }

    if(zeros > 0) return 0;
    
    // check for any possible merges
    for(std::size_t i = 0; i < board.size(); i++) {
        for(std::size_t j = 0; j < board.size(); j++) {
            if(     in(i - 1, 0, board.size()) && in(j,     0, board.size()) && board[i][j] == board[i - 1][j    ]) return 0;
            else if(in(i + 1, 0, board.size()) && in(j,     0, board.size()) && board[i][j] == board[i + 1][j    ]) return 0;
            else if(in(i,     0, board.size()) && in(j - 1, 0, board.size()) && board[i][j] == board[i    ][j - 1]) return 0;
            else if(in(i,     0, board.size()) && in(j + 1, 0, board.size()) && board[i][j] == board[i    ][j + 1]) return 0;
        }
    }

    // no possible merges found, return lose value
    return -1;
}



// got this from cppref, lol
constexpr auto max_str_size = std::numeric_limits<std::streamsize>::max();
char get_input() {
    char c = std::cin.get();
    std::cin.ignore(max_str_size, '\n');
    return c;
}




// ------ DEBUG STUFF ------ // 

void print_board(std::vector<std::vector<int>> &board) {
    cout << "\t";
    for(std::size_t i = 0; i < board.size(); i++) cout << "\t" << i;
    cout << endl << endl;

    for(std::size_t i = 0; i < board.size(); i++) {
        cout << i << ":\t[";
        for(std::size_t j = 0; j < board.size(); j++) {
            cout << "\t" << board[i][j];
        }
        cout << "\t]" << endl;
    }
    cout << endl;
}