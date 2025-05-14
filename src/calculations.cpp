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
        {L'Z', 8},
        {L'a', 1},
        {L'd', 1},
        {L'e', 1},
        {L'i', 1},
        {L'n', 1},
        {L'r', 1},
        {L's', 1},
        {L't', 1},
        {L'g', 2},
        {L'l', 2},
        {L'o', 2},
        {L'b', 3},
        {L'f', 3},
        {L'h', 3},
        {L'k', 3},
        {L'm', 3},
        {L'v', 3},
        {L'p', 4},
        {L'u', 4},
        {L'å', 4},
        {L'ä', 4},
        {L'ö', 4},
        {L'j', 7},
        {L'y', 7},
        {L'c', 8},
        {L'x', 8},
        {L'z', 8}};

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

bool isValidSwedishCharacter(wchar_t c)
{
    return (c >= L'A' && c <= L'Z') || (c >= L'a' && c <= L'z') ||
           c == L'Å' || c == L'Ä' || c == L'Ö' ||
           c == L'å' || c == L'ä' || c == L'ö';
}