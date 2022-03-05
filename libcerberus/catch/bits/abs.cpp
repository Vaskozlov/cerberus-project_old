#include "abs.hpp"
#include <cerberus/debug/debug.hpp>
#include <cerberus/number.hpp>

namespace cerb::debug
{
    constexpr auto testAbsOnInts() -> bool
    {
        EXPECT_TRUE(abs(0) == 0);
        EXPECT_TRUE(abs(10) == 10);

        EXPECT_TRUE(abs(-10) == 10);
        EXPECT_TRUE(abs(-10) == 10U);

        return true;
    }

    constexpr auto testAbsOnFloats() -> bool
    {
        EXPECT_TRUE(safeEqual<f32>(abs(0.0f), 0.0f));
        EXPECT_TRUE(safeEqual<f32>(abs(10.0f), 10.0f));

        EXPECT_TRUE(safeEqual<f32>(abs(-0.0f), 0.0f));
        EXPECT_TRUE(safeEqual<f32>(abs(-10.0f), 10.0f));

        EXPECT_TRUE(safeEqual<f32>(abs(0.0f), 0.0f));
        EXPECT_TRUE(safeEqual<f32>(abs(10.0f), 10.0f));

        EXPECT_TRUE(safeEqual<f64>(abs(-0.0), 0.0));
        EXPECT_TRUE(safeEqual<f64>(abs(-10.0), 10.0));

        return true;
    }

    auto testAbs() -> int
    {
        CR_EXPECT_TRUE(testAbsOnInts());
        CR_EXPECT_TRUE(testAbsOnFloats());
        return 0;
    }
}// namespace cerb::debug
