#include <vector>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <locale>
#include <unordered_set>
#include <chrono>
#include <filesystem>

#include "permutations.h"
#include "board.h"

using namespace std::chrono;

int main(int argc, char const *argv[])
{
    std::locale::global(std::locale("sv_SE.utf8"));
    std::wcout.imbue(std::locale());

    if (!std::filesystem::exists("../output"))
        std::filesystem::create_directory("../output");

    std::vector<wchar_t> rack;
    {
        if (argc > 1)
        {
            std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
            std::wstring ws = conv.from_bytes(argv[1]);
            rack.assign(ws.begin(), ws.end());
        }
        else
        {
            rack = {L'k', L'a', L't', L't', L'e'};
        }
    }

    Board board;
    board.read_board("../output/board.txt");

    auto t0 = high_resolution_clock::now();
    auto plays = board.generate_plays(rack);
    auto t1 = high_resolution_clock::now();

    std::wcout << plays.size() << L" legal plays found in "
               << duration_cast<milliseconds>(t1 - t0).count() << L" ms\n";

    std::sort(plays.begin(), plays.end(),
              [&](const Play &a, const Play &b)
              {
                  return board.get_play_points(a) > board.get_play_points(b);
              });

    std::wofstream out("../output/plays.txt");
    out.imbue(std::locale());
    out << L"# dir = H(horizontal) / V(vertical), pts = crude score\n\n";

    const std::size_t show = std::min<std::size_t>(50, plays.size());
    for (std::size_t i = 0; i < show; ++i)
    {
        const auto &p = plays[i];
        int pts = board.get_play_points(p);

        out << std::setw(2) << i + 1 << L") "
            << (p.dir == HORIZONTAL ? L'H' : L'V')
            << L", pts=" << std::setw(3) << pts << L"  : ";

        for (const auto &pos : p.positions)
        {
            out << L'(' << pos.x << L',' << pos.y << L',' << pos.value << L") ";
        }
        out << L'\n';
    }

    std::wcout << L"Top " << show << L" plays written to ../output/plays.txt\n";
    return 0;
}