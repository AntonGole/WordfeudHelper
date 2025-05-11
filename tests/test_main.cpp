#include <gtest/gtest.h>
#include <locale>
#include <vector>
#include <unordered_set>
#include "permutations.h"

using namespace std;

TEST(DictionaryTest, ContainsKnownWord)
{
    auto dict = get_dictionary();
    EXPECT_TRUE(dict.find(L"användargränssnitt") != dict.end());
    EXPECT_TRUE(dict.find(L"jägarkårer") != dict.end());
    EXPECT_TRUE(dict.find(L"piratpartisten") != dict.end());
    EXPECT_TRUE(dict.find(L"tillstår") != dict.end());
    EXPECT_TRUE(dict.find(L"abbedissan") != dict.end());
    EXPECT_TRUE(dict.find(L"övningskörts") != dict.end());
}

TEST(DictionaryTest, DoesNotContainFakeWord)
{
    auto dict = get_dictionary();
    EXPECT_TRUE(dict.find(L"blååårgh") == dict.end());
    EXPECT_TRUE(dict.find(L"äölsduht") == dict.end());
    EXPECT_TRUE(dict.find(L"övningskördas") == dict.end());
    EXPECT_TRUE(dict.find(L"test ") == dict.end());
    EXPECT_TRUE(dict.find(L" test") == dict.end());
    EXPECT_TRUE(dict.find(L"gökgök") == dict.end());
}

TEST(WordGenerationTest, DoesGenerateAllPossibleWords)
{
    locale::global(locale("sv_SE.utf8"));

    vector<wchar_t> rack = {L'a', L'l', 'k', 'e'};

    auto valid_words = get_valid_words(rack);

    unordered_set<wstring> expected = {
        L"al",
        L"ale",
        L"ek",
        L"eka",
        L"el",
        L"elak",
        L"la",
        L"kl",
        L"kal",
        L"kale",
        L"kela",
        L"kl",
        L"le",
        L"leka",
        L"lek",
        L"kel"};

    EXPECT_EQ(expected, valid_words);
}

TEST(WordGenerationTest, DoesNotGenerateDuplicates)
{
    locale::global(locale("sv_SE.utf8"));

    vector<wchar_t> rack = {L'a', L'n', L'n'};

    auto valid_words = get_valid_words(rack);

    unordered_set<wstring> expected = {
        L"an",
    };

    EXPECT_EQ(expected, valid_words);
}

TEST(EdgeCaseTest, EmptyRackProducesNoWords)
{
    locale::global(locale("sv_SE.utf8"));
    vector<wchar_t> rack;
    auto words = get_valid_words(rack);
    EXPECT_TRUE(words.empty());
}

TEST(EdgeCaseTest, SingleLetterRackProducesNoWords)
{
    locale::global(locale("sv_SE.utf8"));
    vector<wchar_t> rack = {L'a'};
    auto words = get_valid_words(rack);
    EXPECT_TRUE(words.empty());
}