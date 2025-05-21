#include <locale>
#include <codecvt>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <unordered_set>

#include "board.h"
#include "calculations.h"
#include "permutations.h"

using namespace std;

Board::Board()
{
    this->dict = get_dictionary();
}

Board::Board(const vector<vector<wchar_t>> &board)
{
    this->board = board;
    this->dict = get_dictionary();
}

void Board::read_board(const string &filename)
{
    wifstream file(filename);
    file.imbue(locale("sv_SE.utf8"));
    wstring line;

    board.clear();
    while (getline(file, line))
    {
        board.push_back(vector<wchar_t>(line.begin(), line.end()));
    }
}

void Board::print() const
{
    wcout << L"    ";
    for (int i = 0; i < 15; ++i)
        wcout << setw(3) << i;
    wcout << endl;

    wcout << L"   +" << wstring(15 * 3, L'-') << L"+" << endl;

    for (size_t y = 0; y < board.size(); ++y)
    {
        wcout << setw(2) << y << L" |";
        for (wchar_t ch : board[y])
        {
            std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
            std::string s = conv.to_bytes(ch == L' ' ? L'.' : ch);
            std::cout << std::setw(3) << s;
        }
        wcout << L"|" << endl;
    }

    wcout << L"   +" << wstring(15 * 3, L'-') << L"+" << endl;
}

vector<wstring> Board::get_words_from_play(const Play &play)
{
    vector<wstring> created_words;

    if (play.dir == HORIZONTAL)
        created_words.push_back(get_horizontal_word(play.positions[0]));
    else
        created_words.push_back(get_vertical_word(play.positions[0]));
    for (const auto &pos : play.positions)
    {
        vector<wstring> words_from_position = get_words_from_position(pos, play.dir);

        created_words.insert(created_words.end(), words_from_position.begin(), words_from_position.end());
    }

    return created_words;
}

bool Board::is_valid_word(const wstring &word)
{
    return dict.find(word) != dict.end();
}

bool Board::is_valid_play(const Play &play)
{
    if (play.positions.size() < 2)
        return false;
    for (const auto &pos : play.positions)
    {
        if (!is_playable_square(pos))
            return false;
    }
    vector<wstring> created_words = get_words_from_play(play);

    for (const auto &word : created_words)
    {
        if (!is_valid_word(word))
            return false;
    }

    return true;
}

int Board::get_play_points(const Play &play)
{
    int points = 0;

    vector<wstring> words = get_words_from_play(play);

    for (const auto word : words)
    {
        points = points + get_word_points(word);
    }

    return points;
}

vector<Position> Board::get_start_squares()
{
    vector<Position> start_squares;

    for (int x = 0; x < 15; ++x)
    {
        for (int y = 0; y < 15; ++y)
        {
            Position pos = Position{x, y};
            if (is_valid_start_square(pos))
                start_squares.push_back(pos);
        }
    }

    return start_squares;
}

vector<wstring> Board::get_words_from_position(const Position &pos, const Direction &dir)
{
    vector<wstring> words;

    wstring vertical = get_vertical_word(pos);
    wstring horizontal = get_horizontal_word(pos);

    if (vertical.size() > 1 && dir == HORIZONTAL)
        words.push_back(get_vertical_word(pos));
    if (horizontal.size() > 1 && dir == VERTICAL)
        words.push_back(get_horizontal_word(pos));

    return words;
}

wstring Board::get_horizontal_word(const Position &pos)
{
    Position leftmost_pos = pos;
    wstring word;

    while (in_bounds(get_west_position(leftmost_pos)) && is_played(get_west_position(leftmost_pos)))
    {
        leftmost_pos = get_west_position(leftmost_pos);
    }

    Position current_pos = leftmost_pos;

    do
    {
        word.push_back(get_value(current_pos));
        current_pos = get_east_position(current_pos);

    } while (in_bounds(current_pos) && is_played(current_pos));

    return word;
}

wstring Board::get_vertical_word(const Position &pos)
{
    Position uppermost_pos = pos;
    wstring word;

    while (in_bounds(get_north_position(uppermost_pos)) && is_played(get_north_position(uppermost_pos)))
    {
        uppermost_pos = get_north_position(uppermost_pos);
    }

    Position current_pos = uppermost_pos;

    do
    {
        word.push_back(get_value(current_pos));
        current_pos = get_south_position(current_pos);

    } while (in_bounds(current_pos) && is_played(current_pos));

    return word;
}

Position Board::get_north_position(const Position &pos)
{
    return Position{pos.x - 1, pos.y};
}

Position Board::get_east_position(const Position &pos)
{
    return Position{pos.x, pos.y + 1};
}

Position Board::get_south_position(const Position &pos)
{
    return Position{pos.x + 1, pos.y};
}

Position Board::get_west_position(const Position &pos)
{
    return Position{pos.x, pos.y - 1};
}

wchar_t Board::get_value(const Position &pos)
{
    return board[pos.x][pos.y];
}

bool Board::in_bounds(const Position &pos)
{
    return pos.x < 15 && pos.x >= 0 && pos.y < 15 && pos.y >= 0;
}

bool Board::is_empty(const Position &pos)
{
    return !isValidSwedishCharacter(get_value(pos));
}

bool Board::is_played(const Position &pos)
{
    return isValidSwedishCharacter(get_value(pos));
}

bool Board::is_playable_square(const Position &pos)
{
    return in_bounds(pos) && is_empty(pos);
}

bool Board::is_valid_start_square(const Position &pos)
{
    return in_bounds(pos) &&
           is_played(pos) &&
           (is_playable_square(get_north_position(pos)) ||
            is_playable_square(get_east_position(pos)) ||
            is_playable_square(get_south_position(pos)) ||
            is_playable_square(get_west_position(pos)));
}