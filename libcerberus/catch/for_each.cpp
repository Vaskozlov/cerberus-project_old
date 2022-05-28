#include <cerberus/debug/debug.hpp>

namespace cerb::debug
{
    CERBERUS_TEST_FUNC(testForEachOnInts)
    {
        int counter = 0;

        // NOLINTBEGIN magic numbers
        forEach(
            [&counter](int elem) {
                ASSERT_EQUAL(elem, counter * 10);
                ++counter;
            },
            0,
            10,
            20,
            30,
            40,
            50);
        // NOLINTEND

        ASSERT_EQUAL(counter, 6);
        return true;
    }

    auto testForEach() -> int
    {
        CERBERUS_TEST(testForEachOnInts());
        return 0;
    }
}// namespace cerb::debug
