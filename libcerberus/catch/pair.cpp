#include "pair.hpp"
#include <vector>

namespace cerb::test {
    auto pairTest() -> int
    {
        {
            Pair<int, int> c_pair = cerb::make_pair(10, 10);

            EXPECT_TRUE(c_pair.first == 10 && c_pair.second == 10);
            EXPECT_TRUE(c_pair == std::make_pair(10, 10));

            const Pair<int, int> c_pair2 = c_pair;

            EXPECT_TRUE(c_pair == c_pair2);

            c_pair.first += 10;
            c_pair.second -= 5;

            EXPECT_FALSE(c_pair == c_pair2);
            EXPECT_TRUE(c_pair.first == 20 && c_pair.second == 5);
            EXPECT_TRUE(c_pair > c_pair2);
            EXPECT_FALSE(c_pair < c_pair2);
            EXPECT_TRUE(c_pair >= c_pair2);
            EXPECT_FALSE(c_pair <= c_pair2);
            EXPECT_TRUE(c_pair != c_pair2);
        }

        {
            Pair<int, int, HowToComparePair::BY_FIRST_VALUE> c_pair = { 10, 10 };

            EXPECT_TRUE(c_pair.first == 10 && c_pair.second == 10);
            EXPECT_TRUE(c_pair == std::make_pair(10, 10));

            const Pair<int, int, HowToComparePair::BY_FIRST_VALUE> c_pair2 = c_pair;

            EXPECT_TRUE(c_pair == c_pair2);

            c_pair.first -= 10;
            c_pair.second += 5;

            EXPECT_FALSE(c_pair == c_pair2);
            EXPECT_TRUE(c_pair.first == 0 && c_pair.second == 15);
            EXPECT_FALSE(c_pair > c_pair2);
            EXPECT_TRUE(c_pair < c_pair2);
            EXPECT_FALSE(c_pair >= c_pair2);
            EXPECT_TRUE(c_pair <= c_pair2);
            EXPECT_TRUE(c_pair != c_pair2);
        }

        {
            Pair<int, int, HowToComparePair::BY_SECOND_VALUE> c_pair = { 10, 10 };

            EXPECT_TRUE(c_pair.first == 10 && c_pair.second == 10);
            EXPECT_TRUE(c_pair == std::make_pair(10, 10));

            const Pair<int, int, HowToComparePair::BY_SECOND_VALUE> pr2 = c_pair;

            EXPECT_TRUE(c_pair == pr2);

            c_pair.first += 10;
            c_pair.second -= 50;

            EXPECT_FALSE(c_pair == pr2);
            EXPECT_TRUE(c_pair.first == 20 && c_pair.second == -40);
            EXPECT_FALSE(c_pair > pr2);
            EXPECT_TRUE(c_pair < pr2);
            EXPECT_FALSE(c_pair >= pr2);
            EXPECT_TRUE(c_pair <= pr2);
            EXPECT_TRUE(c_pair != pr2);
        }
        {
            const char *str = "Hello, World! It's a long string!";

            const std::initializer_list<int> data = { 10, 20, 30, 40 };

            Pair<std::string, std::vector<int>> c_pair = { str, data };

            EXPECT_TRUE(
                c_pair.first == std::string(str) && c_pair.second == std::vector<int>(data));

            Pair<std::string, std::vector<int>> c_pair2 = std::move(c_pair);

            EXPECT_TRUE(
                c_pair2.first == std::string(str) && c_pair2.second == std::vector<int>(data));
            EXPECT_TRUE(c_pair.first.empty() && c_pair.second.empty());
        }

        return 0;
    }
}// namespace cerb::test