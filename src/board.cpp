#include <locale>
#include <codecvt>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <array>
#include <set>
#include <iterator>

#include "board.h"
#include "calculations.h"
#include "permutations.h"

using namespace std;

Board::Board()
{
    dict = get_dictionary();
}

void Board::read_board(const string &filename)
{
    wifstream file(filename);
    file.imbue(locale("sv_SE.utf8"));
    wstring line;

    board.clear();

    int row = 0;
    while (getline(file, line))
    {
        vector<Position> positions;
        for (int col = 0; col < 15; ++col)
            positions.push_back(Position{row, col, line[col]});
        board.push_back(move(positions));
        ++row;
    }
}

void Board::print() const
{
    wcout << L"    ";
    for (int i = 0; i < 15; ++i)
        wcout << setw(3) << i;
    wcout << L"\n   +" << wstring(15 * 3, L'-') << L"+\n";

    for (size_t r = 0; r < board.size(); ++r)
    {
        wcout << setw(2) << r << L" |";
        for (const Position &p : board[r])
        {
            std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
            std::string s = conv.to_bytes(is_empty(p) ? L'.' : p.value);
            std::cout << std::setw(3) << s;
        }
        wcout << L"|\n";
    }
    wcout << L"   +" << wstring(15 * 3, L'-') << L"+\n";
}

static std::array<int, 0x10000> make_freq(const vector<wchar_t> &rack)
{
    std::array<int, 0x10000> f{};
    for (wchar_t c : rack)
        f[static_cast<unsigned>(c)]++;
    return f;
}

bool Board::vertical_cross_ok(int x, int y, wchar_t ch) const
{
    wstring word;
    for (int r = x - 1; r >= 0 && is_played(board[r][y]); --r)
        word.insert(word.begin(), get_value(board[r][y]));

    word.push_back(ch);

    for (int r = x + 1; r <= 14 && is_played(board[r][y]); ++r)
        word.push_back(get_value(board[r][y]));

    return word.size() == 1 || dict.count(word);
}

bool Board::horizontal_cross_ok(int x, int y, wchar_t ch) const
{
    wstring word;
    for (int c = y - 1; c >= 0 && is_played(board[x][c]); --c)
        word.insert(word.begin(), get_value(board[x][c]));

    word.push_back(ch);

    for (int c = y + 1; c <= 14 && is_played(board[x][c]); ++c)
        word.push_back(get_value(board[x][c]));

    return word.size() == 1 || dict.count(word);
}

vector<Play> Board::generate_plays_horizontal(const vector<wchar_t> &rack)
{
    vector<Play> plays;
    const auto rackFreq = make_freq(rack);
    const auto &F = rackFreq;

    for (const auto &anchor : get_start_squares_horizontal())
    {
        int row = anchor.x;
        int anchorC = anchor.y;

        int left = anchorC;
        while (left > 0 && is_played(board[row][left - 1]))
            --left;
        int right = anchorC;
        while (right < 14 && is_played(board[row][right + 1]))
            ++right;

        wstring fixed;
        for (int c = left; c <= right; ++c)
            fixed.push_back(get_value(board[row][c]));
        int anchorOffset = anchorC - left;

        int spaceLeft = 0;
        for (int c = left - 1; c >= 0 && is_empty(board[row][c]); --c)
            ++spaceLeft;
        int spaceRight = 0;
        for (int c = right + 1; c <= 14 && is_empty(board[row][c]); ++c)
            ++spaceRight;

        int minLen = static_cast<int>(fixed.size());
        int maxLen = minLen + spaceLeft + spaceRight;
        if (maxLen < 2)
            continue;

        for (const auto &w : dict)
        {
            int L = static_cast<int>(w.size());
            if (L < minLen || L > maxLen)
                continue;

            for (size_t pos = w.find(fixed); pos != w.npos; pos = w.find(fixed, pos + 1))
            {
                int anchorIdx = static_cast<int>(pos) + anchorOffset;
                int startCol = anchorC - anchorIdx;
                int endCol = startCol + L - 1;

                if (startCol < 0 || endCol > 14)
                    continue;
                if (startCol - 1 >= 0 && is_played(board[row][startCol - 1]))
                    continue;
                if (endCol + 1 <= 14 && is_played(board[row][endCol + 1]))
                    continue;

                array<int, 0x10000> need{};
                Play p;
                p.dir = HORIZONTAL;
                bool feasible = true;

                for (int k = 0; k < L; ++k)
                {
                    int col = startCol + k;
                    wchar_t boardCh = get_value(board[row][col]);
                    wchar_t wordCh = w[k];

                    if (is_played(board[row][col]))
                    {
                        if (boardCh != wordCh)
                        {
                            feasible = false;
                            break;
                        }
                    }
                    else
                    {
                        need[static_cast<unsigned>(wordCh)]++;
                        if (!vertical_cross_ok(row, col, wordCh))
                        {
                            feasible = false;
                            break;
                        }
                        p.positions.push_back(Position{row, col, wordCh});
                    }
                }

                if (!feasible || p.positions.size() < 2)
                    continue;

                for (size_t i = 0; i < need.size(); ++i)
                    if (need[i] > F[i])
                    {
                        feasible = false;
                        break;
                    }

                if (feasible)
                    plays.push_back(std::move(p));
            }
        }
    }
    return plays;
}

vector<Play> Board::generate_plays_vertical(const vector<wchar_t> &rack)
{
    vector<Play> plays;
    const auto rackFreq = make_freq(rack);
    const auto &F = rackFreq;

    for (const auto &anchor : get_start_squares_vertical())
    {
        int col = anchor.y;
        int anchorR = anchor.x;

        int top = anchorR;
        while (top > 0 && is_played(board[top - 1][col]))
            --top;
        int bot = anchorR;
        while (bot < 14 && is_played(board[bot + 1][col]))
            ++bot;

        wstring fixed;
        for (int r = top; r <= bot; ++r)
            fixed.push_back(get_value(board[r][col]));
        int anchorOffset = anchorR - top;

        int spaceUp = 0;
        for (int r = top - 1; r >= 0 && is_empty(board[r][col]); --r)
            ++spaceUp;
        int spaceDown = 0;
        for (int r = bot + 1; r <= 14 && is_empty(board[r][col]); ++r)
            ++spaceDown;

        int minLen = static_cast<int>(fixed.size());
        int maxLen = minLen + spaceUp + spaceDown;
        if (maxLen < 2)
            continue;

        for (const auto &w : dict)
        {
            int L = static_cast<int>(w.size());
            if (L < minLen || L > maxLen)
                continue;

            for (size_t pos = w.find(fixed); pos != w.npos; pos = w.find(fixed, pos + 1))
            {
                int anchorIdx = static_cast<int>(pos) + anchorOffset;
                int startRow = anchorR - anchorIdx;
                int endRow = startRow + L - 1;

                if (startRow < 0 || endRow > 14)
                    continue;
                if (startRow - 1 >= 0 && is_played(board[startRow - 1][col]))
                    continue;
                if (endRow + 1 <= 14 && is_played(board[endRow + 1][col]))
                    continue;

                array<int, 0x10000> need{};
                Play p;
                p.dir = VERTICAL;
                bool feasible = true;

                for (int k = 0; k < L; ++k)
                {
                    int row = startRow + k;
                    wchar_t boardCh = get_value(board[row][col]);
                    wchar_t wordCh = w[k];

                    if (is_played(board[row][col]))
                    {
                        if (boardCh != wordCh)
                        {
                            feasible = false;
                            break;
                        }
                    }
                    else
                    {
                        need[static_cast<unsigned>(wordCh)]++;
                        if (!horizontal_cross_ok(row, col, wordCh))
                        {
                            feasible = false;
                            break;
                        }
                        p.positions.push_back(Position{row, col, wordCh});
                    }
                }

                if (!feasible || p.positions.size() < 2)
                    continue;

                for (size_t i = 0; i < need.size(); ++i)
                    if (need[i] > F[i])
                    {
                        feasible = false;
                        break;
                    }

                if (feasible)
                    plays.push_back(std::move(p));
            }
        }
    }
    return plays;
}

vector<Play> Board::generate_plays(const vector<wchar_t> &rack)
{
    auto h = generate_plays_horizontal(rack);
    auto v = generate_plays_vertical(rack);
    h.insert(h.end(),
             make_move_iterator(v.begin()),
             make_move_iterator(v.end()));
    return h;
}

vector<wstring> Board::get_words_from_play(const Play &play)
{
    vector<wstring> words;
    if (play.dir == HORIZONTAL)
        words.push_back(get_horizontal_word(play.positions[0]));
    else
        words.push_back(get_vertical_word(play.positions[0]));

    for (const auto &pos : play.positions)
    {
        auto extra = get_words_from_position(pos, play.dir);
        words.insert(words.end(), extra.begin(), extra.end());
    }
    return words;
}

bool Board::is_valid_word(const wstring &word) { return dict.count(word); }

bool Board::is_valid_play(const Play &play)
{
    if (play.positions.size() < 2)
        return false;
    for (const auto &p : play.positions)
        if (!is_playable_square(p))
            return false;
    for (const auto &w : get_words_from_play(play))
        if (!is_valid_word(w))
            return false;
    return true;
}

int Board::get_play_points(const Play &play)
{

    auto is_fresh = [&](int x, int y) -> bool
    {
        for (const auto &p : play.positions)
            if (p.x == x && p.y == y)
                return true;
        return false;
    };

    auto letter_at = [&](int x, int y) -> wchar_t
    {
        for (const auto &p : play.positions)
            if (p.x == x && p.y == y)
                return p.value;
        return board[x][y].value;
    };

    auto score_word = [&](int sx, int sy, Direction dir) -> int
    {
        int dx = (dir == VERTICAL ? 1 : 0);
        int dy = (dir == HORIZONTAL ? 1 : 0);

        int x = sx, y = sy;
        while (in_bounds(Position{x - dx, y - dy}) &&
               isValidSwedishCharacter(letter_at(x - dx, y - dy)))
        {
            x -= dx;
            y -= dy;
        }

        int word_mult = 1, word_score = 0, length = 0;

        for (; in_bounds(Position{x, y}) &&
               isValidSwedishCharacter(letter_at(x, y));
             x += dx, y += dy)
        {
            wchar_t ch = letter_at(x, y);
            int char_pts = get_character_points(ch);
            int let_mult = 1;

            if (is_fresh(x, y))
            {
                switch (board[x][y].value)
                {
                case L'0':
                    let_mult = 2;
                    break;
                case L'1':
                    let_mult = 3;
                    break;
                case L'2':
                    word_mult *= 2;
                    break;
                case L'3':
                    word_mult *= 3;
                    break;
                default:
                    break;
                }
            }
            word_score += char_pts * let_mult;
            ++length;
        }
        return (length > 1) ? word_score * word_mult : 0;
    };

    const Position &ref = play.positions.front();

    int total = score_word(ref.x, ref.y, play.dir);

    Direction perp = (play.dir == HORIZONTAL ? VERTICAL : HORIZONTAL);
    for (const auto &p : play.positions)
        total += score_word(p.x, p.y, perp);

    return total;
}

vector<Position> Board::get_start_squares_horizontal()
{
    vector<Position> v;
    for (int x = 0; x < 15; ++x)
        for (int y = 0; y < 15; ++y)
            if (is_valid_start_square_horizontal(Position{x, y}))
                v.push_back(board[x][y]);
    return v;
}

vector<Position> Board::get_start_squares_vertical()
{
    vector<Position> v;
    for (int x = 0; x < 15; ++x)
        for (int y = 0; y < 15; ++y)
            if (is_valid_start_square_vertical(Position{x, y}))
                v.push_back(board[x][y]);
    return v;
}

vector<wstring> Board::get_words_from_position(const Position &pos, const Direction &dir)
{
    vector<wstring> v;
    auto vert = get_vertical_word(pos);
    auto hori = get_horizontal_word(pos);
    if (dir == HORIZONTAL && vert.size() > 1)
        v.push_back(vert);
    if (dir == VERTICAL && hori.size() > 1)
        v.push_back(hori);
    return v;
}

wstring Board::get_horizontal_word(const Position &pos)
{
    Position left = pos;
    while (in_bounds(get_west_position(left)) && is_played(get_west_position(left)))
        left = get_west_position(left);

    wstring w;
    Position cur = left;
    do
    {
        w.push_back(get_value(cur));
        cur = get_east_position(cur);
    } while (in_bounds(cur) && is_played(cur));
    return w;
}

wstring Board::get_vertical_word(const Position &pos)
{
    Position up = pos;
    while (in_bounds(get_north_position(up)) && is_played(get_north_position(up)))
        up = get_north_position(up);

    wstring w;
    Position cur = up;
    do
    {
        w.push_back(get_value(cur));
        cur = get_south_position(cur);
    } while (in_bounds(cur) && is_played(cur));
    return w;
}

Position Board::get_north_position(const Position &p) { return in_bounds(Position{p.x - 1, p.y}) ? board[p.x - 1][p.y] : Position{p.x - 1, p.y}; }
Position Board::get_east_position(const Position &p) { return in_bounds(Position{p.x, p.y + 1}) ? board[p.x][p.y + 1] : Position{p.x, p.y + 1}; }
Position Board::get_south_position(const Position &p) { return in_bounds(Position{p.x + 1, p.y}) ? board[p.x + 1][p.y] : Position{p.x + 1, p.y}; }
Position Board::get_west_position(const Position &p) { return in_bounds(Position{p.x, p.y - 1}) ? board[p.x][p.y - 1] : Position{p.x, p.y - 1}; }

wchar_t Board::get_value(const Position &p) const { return board[p.x][p.y].value; }
bool Board::in_bounds(const Position &p) const { return p.x >= 0 && p.x < 15 && p.y >= 0 && p.y < 15; }
bool Board::is_empty(const Position &p) const { return !isValidSwedishCharacter(get_value(p)); }
bool Board::is_played(const Position &p) const { return isValidSwedishCharacter(get_value(p)); }
bool Board::is_playable_square(const Position &p) const { return in_bounds(p) && is_empty(p); }

bool Board::is_valid_start_square_horizontal(const Position &p)
{
    return in_bounds(p) && is_played(p) &&
           (is_playable_square(get_east_position(p)) ||
            is_playable_square(get_west_position(p)));
}

bool Board::is_valid_start_square_vertical(const Position &p)
{
    return in_bounds(p) && is_played(p) &&
           (is_playable_square(get_north_position(p)) ||
            is_playable_square(get_south_position(p)));
}
