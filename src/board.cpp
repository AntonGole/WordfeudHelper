#include <locale>
#include <codecvt>
#include <fstream>
#include <iostream>
#include <iomanip>
#include "board.h"

using namespace std;

Board::Board(const vector<vector<wchar_t>> &board)
{
    this->board = board;
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