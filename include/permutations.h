#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <locale>

using namespace std;

unordered_set<wstring> get_valid_words(const vector<wchar_t> &characters);

unordered_set<wstring> get_dictionary();