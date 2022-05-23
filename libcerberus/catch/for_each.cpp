#include <cerberus/debug/debug.hpp>

namespace cerb::debug
{
    CERBERUS_TEST_FUNC(testForEachOnInts)
    {
        int counter = 0;

        // NOLINTBEGIN magic numbers
        forEach(
            [&counter](int elem) {
                EXPECT_EQUAL(elem, counter * 10);
                ++counter;
            },
            0,
            10,
            20,
            30,
            40,
            50);
        // NOLINTEND

        EXPECT_EQUAL(counter, 6);
        return true;
    }

    auto testForEach() -> int
    {
        CERBERUS_TEST(testForEachOnInts());
        return 0;
    }
}// namespace cerb::debug
