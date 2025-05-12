#include "calculations.h"

using namespace std;

int get_character_points(const wchar_t c)
{
    unordered_map<wchar_t, int> points_map = {
        {L'A', 1},
        {L'D', 1},
        {L'E', 1},
        {L'I', 1},
        {L'N', 1},
        {L'R', 1},
        {L'S', 1},
        {L'T', 1},
        {L'G', 2},
        {L'L', 2},
        {L'O', 2},
        {L'B', 3},
        {L'F', 3},
        {L'H', 3},
        {L'K', 3},
        {L'M', 3},
        {L'V', 3},
        {L'P', 4},
        {L'U', 4},
        {L'Å', 4},
        {L'Ä', 4},
        {L'Ö', 4},
        {L'J', 7},
        {L'Y', 7},
        {L'C', 8},
        {L'X', 8},
        {L'Z', 8}};

    auto it = points_map.find(c);
    return (it != points_map.end()) ? it->second : 0;
}

int get_word_points(const wstring str)
{
    int points = 0;

    for (const wchar_t c : str)
    {
        points += get_character_points(c);
    }

    return points;
}
