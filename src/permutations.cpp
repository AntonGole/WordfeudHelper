#include "permutations.h"
#include <fstream>
#include <iostream>
#include <algorithm>

unordered_set<wstring> get_valid_words(const vector<wchar_t> &characters)
{
    auto dict = get_dictionary();

    vector<int> freq(0x10000, 0);
    for (wchar_t c : characters)
    {
        freq[c]++;
    }

    unordered_set<wstring> possible;

    for (const auto &w : dict)
    {
        size_t L = w.size();
        if (L < 2 || L > characters.size())
            continue;

        auto f = freq;
        bool ok = true;
        for (wchar_t c : w)
        {
            if (--f[c] < 0)
            {
                ok = false;
                break;
            }
        }

        if (ok)
        {
            possible.insert(w);
        }
    }

    return possible;
}

unordered_set<wstring> get_dictionary()
{
    unordered_set<wstring> word_set;

    std::wifstream inFile("../data/swedish_words.txt");

    inFile.imbue(std::locale("sv_SE.utf8"));

    if (!inFile)
    {
        std::wcerr << L"Failed to open dictionary file!" << std::endl;
        return {};
    }

    std::wstring line;
    while (std::getline(inFile, line))
    {
        if (!line.empty())
        {
            word_set.insert(line);
        }
    }

    return word_set;
}