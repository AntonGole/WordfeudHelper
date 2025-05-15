#pragma once

#include <vector>
#include <string>
#include <unordered_set>

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
    wchar_t value = L' ';

    void print() const
    {
        wcout << L"Position(" << x << L", " << y << L") = '"
              << (value == L' ' ? L' ' : value) << L"'\n";
    }
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

    void read_board(const string &filename);
    void print() const;

    vector<Play> generate_plays(const vector<wchar_t> &rack);
    vector<Play> generate_plays_horizontal(const vector<wchar_t> &rack);
    vector<Play> generate_plays_vertical(const vector<wchar_t> &rack);

    vector<wstring> get_words_from_play(const Play &play);
    bool is_valid_word(const wstring &word);
    bool is_valid_play(const Play &play);
    int get_play_points(const Play &play);

    vector<Position> get_start_squares_horizontal();
    vector<Position> get_start_squares_vertical();

    vector<wstring> get_words_from_position(const Position &pos,
                                            const Direction &dir);

    wstring get_horizontal_word(const Position &pos);
    wstring get_vertical_word(const Position &pos);

    Position get_north_position(const Position &pos);
    Position get_east_position(const Position &pos);
    Position get_south_position(const Position &pos);
    Position get_west_position(const Position &pos);

    wchar_t get_value(const Position &pos) const;
    bool in_bounds(const Position &pos) const;
    bool is_empty(const Position &pos) const;
    bool is_played(const Position &pos) const;
    bool is_playable_square(const Position &pos) const;

    bool is_valid_start_square_horizontal(const Position &pos);
    bool is_valid_start_square_vertical(const Position &pos);

    bool vertical_cross_ok(int x, int y, wchar_t ch) const;
    bool horizontal_cross_ok(int x, int y, wchar_t ch) const;

    vector<vector<Position>> board;
    unordered_set<wstring> dict;
};
