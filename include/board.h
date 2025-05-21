#pragma once

#include <vector>
#include <string>

using namespace std;

enum Direction
{
    HORIZONTAL = 0,
    VERTICAL = 1
};

struct Position
{
    int x;
    int y;
};

struct Play
{
    vector<Position> positions;
    Direction dir;
};

class Board
{
public:
    Board();
    Board(const vector<vector<wchar_t>> &board);

    void read_board(const string &filename);
    void print() const;

    vector<wstring> get_words_from_play(const Play &play);

    bool is_valid_word(const wstring &word);

    bool is_valid_play(const Play &play);

    vector<Position> get_start_squares();

    int get_play_points(const Play &play);

    vector<wstring> get_words_from_position(const Position &pos, const Direction &dir);

    wstring get_horizontal_word(const Position &pos);

    wstring get_vertical_word(const Position &pos);

    Position get_north_position(const Position &pos);

    Position get_east_position(const Position &pos);

    Position get_south_position(const Position &pos);

    Position get_west_position(const Position &pos);

    wchar_t get_value(const Position &pos);

    bool in_bounds(const Position &pos);

    bool is_empty(const Position &pos);

    bool is_played(const Position &pos);

    bool is_playable_square(const Position &pos);

    bool is_valid_start_square(const Position &pos);

    vector<vector<wchar_t>> board;
    unordered_set<std::wstring> dict;
};