#include <cerberus/debug/debug.hpp>
#include <cerberus/number.hpp>

namespace cerb::debug
{
    constexpr auto testMinMaxOnInts() -> bool
    {
        EXPECT_TRUE(min(10, -41, 0, 14515) == -41);
        EXPECT_TRUE(max(10, -41, 0, 14515) == 14515);
        return true;
    }

    constexpr auto testMinMaxOnFloats() -> bool
    {
        EXPECT_TRUE(safeEqual(min(10.1551, -41.51961, 14515.51616, -5151.5161), -5151.5161));
        EXPECT_TRUE(safeEqual(max(10.1551, -41.51961, 14515.51616, -5151.5161), 14515.51616));
        return true;
    }

    auto testMinMax() -> int
    {
        CR_EXPECT_TRUE(testMinMaxOnInts());
        CR_EXPECT_TRUE(testMinMaxOnFloats());
        return 0;
    }
}// namespace cerb::debug
