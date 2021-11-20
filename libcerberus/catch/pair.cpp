#include "pair.hpp"
#include <vector>

namespace cerb::test {
    auto pair_test() -> int
    {
        {
            Pair<int, int> c_pair = cerb::make_pair(10, 10);

            expect_true(
                [&c_pair] { return c_pair.first == 10 && c_pair.second == 10; },
                CERBLIB_LOCATION);

            expect_true(
                [&c_pair] { return c_pair == std::make_pair(10, 10); }, CERBLIB_LOCATION);

            const Pair<int, int> c_pair2 = c_pair;

            expect_true(
                [&c_pair, &c_pair2] { return c_pair == c_pair2; }, CERBLIB_LOCATION);

            c_pair.first += 10;
            c_pair.second -= 5;

            expect_false(
                [&c_pair, &c_pair2] { return c_pair == c_pair2; }, CERBLIB_LOCATION);

            expect_true(
                [&c_pair] { return c_pair.first == 20 && c_pair.second == 5; },
                CERBLIB_LOCATION);

            expect_true(
                [&c_pair, &c_pair2] { return c_pair > c_pair2; }, CERBLIB_LOCATION);
            expect_false(
                [&c_pair, &c_pair2] { return c_pair < c_pair2; }, CERBLIB_LOCATION);
            expect_true(
                [&c_pair, &c_pair2] { return c_pair >= c_pair2; }, CERBLIB_LOCATION);
            expect_false(
                [&c_pair, &c_pair2] { return c_pair <= c_pair2; }, CERBLIB_LOCATION);
            expect_true(
                [&c_pair, &c_pair2] { return c_pair != c_pair2; }, CERBLIB_LOCATION);
        }

        {
            Pair<int, int, PairComparisonRule::BY_FIRST_VALUE> c_pair = { 10, 10 };

            expect_true(
                [&c_pair] { return c_pair.first == 10 && c_pair.second == 10; },
                CERBLIB_LOCATION);

            expect_true(
                [&c_pair] { return c_pair == std::make_pair(10, 10); }, CERBLIB_LOCATION);

            const Pair<int, int, PairComparisonRule::BY_FIRST_VALUE> c_pair2 = c_pair;

            expect_true(
                [&c_pair, &c_pair2] { return c_pair == c_pair2; }, CERBLIB_LOCATION);

            c_pair.first -= 10;
            c_pair.second += 5;

            expect_false(
                [&c_pair, &c_pair2] { return c_pair == c_pair2; }, CERBLIB_LOCATION);

            expect_true(
                [&c_pair] { return c_pair.first == 0 && c_pair.second == 15; },
                CERBLIB_LOCATION);

            expect_false(
                [&c_pair, &c_pair2] { return c_pair > c_pair2; }, CERBLIB_LOCATION);
            expect_true(
                [&c_pair, &c_pair2] { return c_pair < c_pair2; }, CERBLIB_LOCATION);
            expect_false(
                [&c_pair, &c_pair2] { return c_pair >= c_pair2; }, CERBLIB_LOCATION);
            expect_true(
                [&c_pair, &c_pair2] { return c_pair <= c_pair2; }, CERBLIB_LOCATION);
            expect_true(
                [&c_pair, &c_pair2] { return c_pair != c_pair2; }, CERBLIB_LOCATION);
        }

        {
            Pair<int, int, PairComparisonRule::BY_SECOND_VALUE> c_pair = { 10, 10 };

            expect_true(
                [&c_pair] { return c_pair.first == 10 && c_pair.second == 10; },
                CERBLIB_LOCATION);

            expect_true(
                [&c_pair] { return c_pair == std::make_pair(10, 10); }, CERBLIB_LOCATION);

            const Pair<int, int, PairComparisonRule::BY_SECOND_VALUE> pr2 = c_pair;

            expect_true([&c_pair, &pr2] { return c_pair == pr2; }, CERBLIB_LOCATION);

            c_pair.first += 10;
            c_pair.second -= 50;

            expect_false([&c_pair, &pr2] { return c_pair == pr2; }, CERBLIB_LOCATION);

            expect_true(
                [&c_pair] { return c_pair.first == 20 && c_pair.second == -40; },
                CERBLIB_LOCATION);

            expect_false([&c_pair, &pr2] { return c_pair > pr2; }, CERBLIB_LOCATION);
            expect_true([&c_pair, &pr2] { return c_pair < pr2; }, CERBLIB_LOCATION);
            expect_false([&c_pair, &pr2] { return c_pair >= pr2; }, CERBLIB_LOCATION);
            expect_true([&c_pair, &pr2] { return c_pair <= pr2; }, CERBLIB_LOCATION);
            expect_true([&c_pair, &pr2] { return c_pair != pr2; }, CERBLIB_LOCATION);
        }
        {
            const char *str = "Hello, World! It's a long string!";

            const std::initializer_list<int> data = { 10, 20, 30, 40 };

            Pair<std::string, std::vector<int>> c_pair = { str, data };

            expect_true(
                [&c_pair, &str, &data] {
                    return c_pair.first == std::string(str) &&
                           c_pair.second == std::vector<int>(data);
                },
                CERBLIB_LOCATION);

            Pair<std::string, std::vector<int>> c_pair2 = std::move(c_pair);

            expect_true(
                [&c_pair2, &str, &data] {
                    return c_pair2.first == std::string(str) &&
                           c_pair2.second == std::vector<int>(data);
                },
                CERBLIB_LOCATION);

            expect_true(
                [&c_pair] { return c_pair.first.empty() && c_pair.second.empty(); },
                CERBLIB_LOCATION);
        }

        return 0;
    }
}// namespace cerb::test