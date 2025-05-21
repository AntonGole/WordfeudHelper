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
    {
        std::filesystem::create_directory("../output");
    }

    std::wofstream outFile("../output/result.txt");
    outFile.imbue(std::locale());

    vector<wchar_t> rack = {L'k', L'a', L't', L't', L'e'};
    auto start = high_resolution_clock::now();
    auto valid_words = get_valid_words(rack);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    std::cout << "Time taken: " << duration.count() << " ms" << std::endl;

    for (const auto &word : valid_words)
    {
        outFile << word << L'\n';
    }

    Board board;
    board.read_board("../output/board.txt");

    Play play;
    vector<Position> positions{{Position{4, 3}, Position{4, 4}, Position{4, 5}, Position{4, 6}}};
    play.positions = positions;
    play.dir = HORIZONTAL;

    cout << board.get_play_points(play) << endl;

    board.print();
    return 0;
}