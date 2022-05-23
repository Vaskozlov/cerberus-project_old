#include <cerberus/cerberus.hpp>
#include <cerberus/debug/debug.hpp>
#include <cerberus/pair.hpp>
#include <vector>

#if __cpp_lib_constexpr_vector >= 201907L && !__clang__
#    define CONSTEXPR_VECTOR constexpr
#else
#    define CONSTEXPR_VECTOR
#endif

namespace cerb::debug
{
    CERBERUS_TEST_FUNC(testDefaultPair)
    {
        auto pair = cerb::makePair(10, 10);

        EXPECT_EQUAL(pair.first, 10);
        EXPECT_EQUAL(pair.second, 10);
        EXPECT_EQUAL(pair, std::make_pair(10, 10));

        auto const pair_2 = pair;

        EXPECT_EQUAL(pair, pair_2);

        pair.first += 10;
        pair.second -= 5;

        EXPECT_FALSE(pair == pair_2);
        EXPECT_TRUE(logicalAnd(pair.first == 20, pair.second == 5));
        EXPECT_TRUE(pair > pair_2);
        EXPECT_FALSE(pair < pair_2);
        EXPECT_TRUE(pair >= pair_2);
        EXPECT_FALSE(pair <= pair_2);
        EXPECT_TRUE(pair != pair_2);

        return true;
    }

    CERBERUS_TEST_FUNC(testFirstValuePair)
    {
        auto pair = makePair<PairComparison::BY_FIRST_VALUE>(10, 10);

        EXPECT_EQUAL(pair.first, 10);
        EXPECT_EQUAL(pair.second, 10);
        EXPECT_EQUAL(pair, std::make_pair(10, 10));

        auto c_pair_2 = pair;

        EXPECT_TRUE(pair == c_pair_2);

        pair.first -= 10;
        pair.second += 5;

        EXPECT_NOT_EQUAL(pair, c_pair_2);

        EXPECT_EQUAL(pair.first, 0);
        EXPECT_EQUAL(pair.second, 15);

        EXPECT_FALSE(pair > c_pair_2);
        EXPECT_TRUE(pair < c_pair_2);
        EXPECT_FALSE(pair >= c_pair_2);
        EXPECT_TRUE(pair <= c_pair_2);
        EXPECT_TRUE(pair != c_pair_2);

        return true;
    }

    CERBERUS_TEST_FUNC(testSecondValuePair)
    {
        auto pair = makePair<PairComparison::BY_SECOND_VALUE>(10, 10);

        EXPECT_EQUAL(pair.first, 10);
        EXPECT_EQUAL(pair.second, 10);
        EXPECT_EQUAL(pair, std::make_pair(10, 10));

        auto pair_2 = pair;

        EXPECT_EQUAL(pair, pair_2);

        pair.first += 10;
        pair.second -= 50;

        EXPECT_NOT_EQUAL(pair, pair_2);

        EXPECT_EQUAL(pair.first, 20);
        EXPECT_EQUAL(pair.second, -40);

        EXPECT_FALSE(pair > pair_2);
        EXPECT_TRUE(pair < pair_2);
        EXPECT_FALSE(pair >= pair_2);
        EXPECT_TRUE(pair <= pair_2);
        EXPECT_TRUE(pair != pair_2);

        return true;
    }

    CONSTEXPR_VECTOR auto testPairOnStringAndVector() -> bool
    {
        char const *str = "Hello, World! It's a long string!";
        std::initializer_list<int> const data = { 10, 20, 30, 40 };
        Pair<std::string, std::vector<int>> pair = { str, data };

        EXPECT_EQUAL(pair.first, str);
        EXPECT_EQUAL(pair.second, std::vector<int>(data));

        auto pair_2 = std::move(pair);

        EXPECT_EQUAL(pair_2.first, str);
        EXPECT_EQUAL(pair_2.second, std::vector<int>(data));

        EXPECT_TRUE(pair.first.empty()); // NOLINT
        EXPECT_TRUE(pair.second.empty());// NOLINT

        return true;
    }

    auto testPair() -> int
    {
        CERBERUS_TEST(testDefaultPair());
        CERBERUS_TEST(testFirstValuePair());
        CERBERUS_TEST(testSecondValuePair());

#if __cpp_lib_constexpr_vector >= 201907L && !__clang__
        CERBERUS_TEST(testPairOnStringAndVector());
#else
        EXPECT_TRUE(testPairOnStringAndVector());
#endif

        return 0;
    }
}// namespace cerb::debug