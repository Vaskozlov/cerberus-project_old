#include "for_each.hpp"

namespace cerb::debug
{
    constexpr auto testForEachOnInts() -> bool
    {
        int counter = 0;

        forEach(
            [&counter](int elem) {
                EXPECT_TRUE(elem == counter * 10);
                ++counter;
            },
            0,
            10,
            20,
            30,
            40,
            50);

        EXPECT_TRUE(counter == 6);

        return true;
    }

    auto testForEach(u32) -> int
    {
        CR_EXPECT_TRUE(testForEachOnInts());
        return 0;
    }
}// namespace cerb::debug
