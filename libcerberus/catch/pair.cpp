#include "pair.hpp"
#include <vector>

namespace cerb::test {
    auto pairTest() -> int
    {
        {
            auto pair = cerb::makePair(10, 10);

            EXPECT_TRUE(pair.first == 10 && pair.second == 10);
            EXPECT_TRUE(pair == std::make_pair(10, 10));

            const auto pair_2 = pair;

            EXPECT_TRUE(pair == pair_2);

            pair.first += 10;
            pair.second -= 5;

            EXPECT_FALSE(pair == pair_2);
            EXPECT_TRUE(pair.first == 20 && pair.second == 5);
            EXPECT_TRUE(pair > pair_2);
            EXPECT_FALSE(pair < pair_2);
            EXPECT_TRUE(pair >= pair_2);
            EXPECT_FALSE(pair <= pair_2);
            EXPECT_TRUE(pair != pair_2);
        }
        {
            auto pair = makePair<HowToComparePair::BY_FIRST_VALUE>(10, 10);

            EXPECT_TRUE(pair.first == 10 && pair.second == 10);
            EXPECT_TRUE(pair == std::make_pair(10, 10));

            auto c_pair_2 = pair;

            EXPECT_TRUE(pair == c_pair_2);

            pair.first -= 10;
            pair.second += 5;

            EXPECT_FALSE(pair == c_pair_2);
            EXPECT_TRUE(pair.first == 0 && pair.second == 15);
            EXPECT_FALSE(pair > c_pair_2);
            EXPECT_TRUE(pair < c_pair_2);
            EXPECT_FALSE(pair >= c_pair_2);
            EXPECT_TRUE(pair <= c_pair_2);
            EXPECT_TRUE(pair != c_pair_2);
        }

        {
            auto pair = makePair<HowToComparePair::BY_SECOND_VALUE>(10, 10);

            EXPECT_TRUE(pair.first == 10 && pair.second == 10);
            EXPECT_TRUE(pair == std::make_pair(10, 10));

            auto pair_2 = pair;

            EXPECT_TRUE(pair == pair_2);

            pair.first += 10;
            pair.second -= 50;

            EXPECT_FALSE(pair == pair_2);
            EXPECT_TRUE(pair.first == 20 && pair.second == -40);
            EXPECT_FALSE(pair > pair_2);
            EXPECT_TRUE(pair < pair_2);
            EXPECT_FALSE(pair >= pair_2);
            EXPECT_TRUE(pair <= pair_2);
            EXPECT_TRUE(pair != pair_2);
        }

        {
            const char *str                            = "Hello, World! It's a long string!";
            const std::initializer_list<int> data      = { 10, 20, 30, 40 };
            Pair<std::string, std::vector<int>> pair   = { str, data };

            EXPECT_TRUE(pair.first == std::string(str) && pair.second == std::vector<int>(data));

            auto pair_2 = std::move(pair);

            EXPECT_TRUE(
                pair_2.first == std::string(str) && pair_2.second == std::vector<int>(data));
            EXPECT_TRUE(pair.first.empty() && pair.second.empty());
        }

        return 0;
    }
}// namespace cerb::test