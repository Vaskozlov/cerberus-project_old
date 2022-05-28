#include <cerberus/cerberus.hpp>
#include <cerberus/debug/debug.hpp>
#include <cerberus/pair.hpp>
#include <vector>

namespace cerb::debug
{
    CERBERUS_TEST_FUNC(testDefaultPair)
    {
        auto pair = cerb::makePair(10, 10);

        ASSERT_EQUAL(pair.first, 10);
        ASSERT_EQUAL(pair.second, 10);
        ASSERT_EQUAL(pair, std::make_pair(10, 10));

        auto const pair_2 = pair;

        ASSERT_EQUAL(pair, pair_2);

        pair.first += 10;
        pair.second -= 5;

        ASSERT_FALSE(pair == pair_2);
        ASSERT_TRUE(logicalAnd(pair.first == 20, pair.second == 5));
        ASSERT_TRUE(pair > pair_2);
        ASSERT_FALSE(pair < pair_2);
        ASSERT_TRUE(pair >= pair_2);
        ASSERT_FALSE(pair <= pair_2);
        ASSERT_TRUE(pair != pair_2);

        return true;
    }

    CERBERUS_TEST_FUNC(testFirstValuePair)
    {
        auto pair = makePair<PairComparison::BY_FIRST_VALUE>(10, 10);

        ASSERT_EQUAL(pair.first, 10);
        ASSERT_EQUAL(pair.second, 10);
        ASSERT_EQUAL(pair, std::make_pair(10, 10));

        auto c_pair_2 = pair;

        ASSERT_TRUE(pair == c_pair_2);

        pair.first -= 10;
        pair.second += 5;

        ASSERT_NOT_EQUAL(pair, c_pair_2);

        ASSERT_EQUAL(pair.first, 0);
        ASSERT_EQUAL(pair.second, 15);

        ASSERT_FALSE(pair > c_pair_2);
        ASSERT_TRUE(pair < c_pair_2);
        ASSERT_FALSE(pair >= c_pair_2);
        ASSERT_TRUE(pair <= c_pair_2);
        ASSERT_TRUE(pair != c_pair_2);

        return true;
    }

    CERBERUS_TEST_FUNC(testSecondValuePair)
    {
        auto pair = makePair<PairComparison::BY_SECOND_VALUE>(10, 10);

        ASSERT_EQUAL(pair.first, 10);
        ASSERT_EQUAL(pair.second, 10);
        ASSERT_EQUAL(pair, std::make_pair(10, 10));

        auto pair_2 = pair;

        ASSERT_EQUAL(pair, pair_2);

        pair.first += 10;
        pair.second -= 50;

        ASSERT_NOT_EQUAL(pair, pair_2);

        ASSERT_EQUAL(pair.first, 20);
        ASSERT_EQUAL(pair.second, -40);

        ASSERT_FALSE(pair > pair_2);
        ASSERT_TRUE(pair < pair_2);
        ASSERT_FALSE(pair >= pair_2);
        ASSERT_TRUE(pair <= pair_2);
        ASSERT_TRUE(pair != pair_2);

        return true;
    }

    CERBERUS_TEST_FUNC_WITH_CONSTEXPR_VECTOR(testPairOnStringAndVector)
    {
        char const *str = "Hello, World! It's a long string!";
        std::initializer_list<int> const data = { 10, 20, 30, 40 };
        Pair<std::string, std::vector<int>> pair = { str, data };

        ASSERT_EQUAL(pair.first, str);
        ASSERT_EQUAL(pair.second, std::vector<int>(data));

        auto pair_2 = std::move(pair);

        ASSERT_EQUAL(pair_2.first, str);
        ASSERT_EQUAL(pair_2.second, std::vector<int>(data));

        ASSERT_TRUE(pair.first.empty()); // NOLINT
        ASSERT_TRUE(pair.second.empty());// NOLINT

        return true;
    }

    auto testPair() -> int
    {
        CERBERUS_TEST(testDefaultPair());
        CERBERUS_TEST(testFirstValuePair());
        CERBERUS_TEST(testSecondValuePair());

        CERBERUS_TEST_FOR_CONSTEXPR_VECTOR(testPairOnStringAndVector());

        return 0;
    }
}// namespace cerb::debug