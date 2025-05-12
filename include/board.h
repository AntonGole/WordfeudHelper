#pragma once

#include <vector>
#include <string>

using namespace std;

class Board
{
public:
    Board() = default;
    Board(const vector<vector<wchar_t>> &board);

    void read_board(const string &filename);
    void print() const;

private:
    vector<vector<wchar_t>> board;
};