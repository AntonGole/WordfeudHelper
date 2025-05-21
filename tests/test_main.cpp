#include <gtest/gtest.h>
#include <locale>
#include <vector>
#include <unordered_set>
#include "permutations.h"
#include "board.h"

using namespace std;

static vector<vector<wchar_t>> make_empty_board()
{
    return vector<vector<wchar_t>>(15, vector<wchar_t>(15, L' '));
}

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

TEST(BoardFunctionTest, DirectionalPositionHelpers)
{
    Board board;
    Position p{5, 5};

    Position north = board.get_north_position(p);
    EXPECT_EQ(north.x, 4);
    EXPECT_EQ(north.y, 5);

    Position south = board.get_south_position(p);
    EXPECT_EQ(south.x, 6);
    EXPECT_EQ(south.y, 5);

    Position east = board.get_east_position(p);
    EXPECT_EQ(east.x, 5);
    EXPECT_EQ(east.y, 6);

    Position west = board.get_west_position(p);
    EXPECT_EQ(west.x, 5);
    EXPECT_EQ(west.y, 4);
}

TEST(BoardFunctionTest, InBoundsCatchesEdges)
{
    Board board;
    EXPECT_TRUE(board.in_bounds(Position{0, 0}));
    EXPECT_TRUE(board.in_bounds(Position{14, 14}));
    EXPECT_FALSE(board.in_bounds(Position{-1, 0}));
    EXPECT_FALSE(board.in_bounds(Position{0, -1}));
    EXPECT_FALSE(board.in_bounds(Position{15, 5}));
    EXPECT_FALSE(board.in_bounds(Position{5, 15}));
}

TEST(BoardFunctionTest, GetValue)
{
    Board board;
    auto b = make_empty_board();
    b[2][3] = L'Å';
    board.board = b;
    EXPECT_EQ(board.get_value(Position{2, 3}), L'Å');
}

TEST(BoardFunctionTest, EmptyAndPlayedAndPlayable)
{
    Board board;
    board.board = make_empty_board();

    EXPECT_TRUE(board.is_empty(Position{7, 7}));
    EXPECT_FALSE(board.is_played(Position{7, 7}));
    EXPECT_TRUE(board.is_playable_square(Position{7, 7}));

    board.board[7][7] = L'a';
    EXPECT_FALSE(board.is_empty(Position{7, 7}));
    EXPECT_TRUE(board.is_played(Position{7, 7}));
    EXPECT_FALSE(board.is_playable_square(Position{7, 7}));
}

TEST(BoardFunctionTest, IsValidStartSquare)
{
    Board board;
    board.board = make_empty_board();

    EXPECT_FALSE(board.is_valid_start_square(Position{7, 7}));

    board.board[7][7] = L'b';

    EXPECT_TRUE(board.is_valid_start_square(Position{7, 7}));

    EXPECT_FALSE(board.is_valid_start_square(Position{6, 6}));
    EXPECT_FALSE(board.is_valid_start_square(Position{0, 0}));
}

TEST(BoardFunctionTest, GetStartSquaresOnEmptyAndSingleTile)
{
    Board board;
    board.board = make_empty_board();

    auto starts = board.get_start_squares();
    EXPECT_TRUE(starts.empty());

    board.board[7][7] = L'x';

    starts = board.get_start_squares();
    ASSERT_EQ(starts.size(), 1u);
    EXPECT_EQ(starts[0].x, 7);
    EXPECT_EQ(starts[0].y, 7);

    EXPECT_TRUE(board.is_valid_start_square(Position{7, 7}));

    EXPECT_FALSE(board.is_valid_start_square(Position{6, 7}));
    EXPECT_FALSE(board.is_valid_start_square(Position{8, 7}));
    EXPECT_FALSE(board.is_valid_start_square(Position{7, 6}));
    EXPECT_FALSE(board.is_valid_start_square(Position{7, 8}));
}

TEST(BoardFunctionTest, GetHorizontalAndVerticalWord)
{
    Board board;

    vector<vector<wchar_t>> b = {
        {L'A', L'B', L'C'},
        {L' ', L'X', L' '},
        {L' ', L'Y', L' '},
    };

    auto full = make_empty_board();
    for (int x = 0; x < 3; ++x)
        for (int y = 0; y < 3; ++y)
            full[x][y] = b[x][y];
    board.board = full;

    auto hw = board.get_horizontal_word(Position{0, 1});
    EXPECT_EQ(hw, L"ABC");

    auto vw = board.get_vertical_word(Position{1, 1});
    EXPECT_EQ(vw, L"BXY");
}

TEST(BoardFunctionTest, GetWordsFromPosition)
{
    Board board;
    auto full = make_empty_board();
    full[0][0] = L'A';
    full[0][1] = L'B';
    full[0][2] = L'C';
    full[1][1] = L'X';
    full[2][1] = L'Y';
    board.board = full;

    auto c1 = board.get_words_from_position(Position{0, 1}, HORIZONTAL);
    ASSERT_EQ(c1.size(), 1u);
    EXPECT_EQ(c1[0], L"BXY");

    auto c2 = board.get_words_from_position(Position{1, 1}, VERTICAL);
    EXPECT_TRUE(c2.empty());

    full[1][0] = L'D';
    full[1][2] = L'E';
    board.board = full;

    auto c3 = board.get_words_from_position(Position{1, 1}, VERTICAL);
    ASSERT_EQ(c3.size(), 1u);
    EXPECT_EQ(c3[0], L"DXE");
}

TEST(BoardFunctionTest, IsValidWordUsesDictionary)
{
    Board board;

    EXPECT_TRUE(board.is_valid_word(L"användargränssnitt"));
    EXPECT_FALSE(board.is_valid_word(L"blååårgh"));
}