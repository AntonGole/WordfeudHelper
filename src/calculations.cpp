#include "calculations.h"
#include <unordered_map>

using namespace std;

int get_character_points(const char c)
{
    unordered_map<char, int> points_map = {
        {'A', 1},
        {'D', 1},
        {'E', 1},
        {'I', 1},
        {'N', 1},
        {'R', 1},
        {'S', 1},
        {'T', 1},
        {'G', 2},
        {'L', 2},
        {'O', 2},
        {'B', 3},
        {'F', 3},
        {'H', 3},
        {'K', 3},
        {'M', 3},
        {'V', 3},
        {'P', 4},
        {'U', 4},
        {'Å', 4},
        {'Ä', 4},
        {'Ö', 4},
        {'J', 7},
        {'U', 7},
        {'C', 8},
        {'X', 8},
        {'Z', 8}};

    return points_map[c];
}

int get_word_points(const string str)
{
    int points = 0;

    for (const char c : str)
    {
        points += get_character_points(c);
    }

    return points;
}
