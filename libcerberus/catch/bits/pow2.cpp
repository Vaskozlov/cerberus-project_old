#include <cerberus/debug/debug.hpp>
#include <cerberus/number.hpp>

namespace cerb::debug
{
    CERBLIB_DECL auto testPow2OnInt() -> bool
    {
        EXPECT_TRUE(pow2<u32>(0U) == 1U);
        EXPECT_TRUE(pow2<u32>(2U) == 4U);
        EXPECT_TRUE(pow2<u32>(10U) == 1024U);
        EXPECT_TRUE(pow2<u64>(54ULL) == (1ULL << 54ULL));
        return true;
    }

    CERBLIB_DECL auto testPow2OnF32() -> bool
    {
        EXPECT_TRUE(safeEqual<f32>(pow2<f32>(0U), 1.0f));
        EXPECT_TRUE(safeEqual<f32>(pow2<f32>(2U), 4.0f));
        EXPECT_TRUE(safeEqual<f32>(pow2<f32>(10U), 1024.0f));
        EXPECT_TRUE(safeEqual<f32>(pow2<f32>(54ULL), static_cast<f32>((1ULL << 54ULL))));
        return true;
    }

    CERBLIB_DECL auto testPow2OnF64() -> bool
    {
        EXPECT_TRUE(safeEqual<f64>(pow2<f64>(0U), 1.0));
        EXPECT_TRUE(safeEqual<f64>(pow2<f64>(2U), 4.0));
        EXPECT_TRUE(safeEqual<f64>(pow2<f64>(10U), 1024.0));
        EXPECT_TRUE(safeEqual<f64>(pow2<f64>(54ULL), static_cast<f64>((1ULL << 54ULL))));
        return true;
    }

    auto testPow2() -> int
    {
        CR_EXPECT_TRUE(testPow2OnInt());
        CR_EXPECT_TRUE(testPow2OnF32());
        CR_EXPECT_TRUE(testPow2OnF64());
        return 0;
    }
}// namespace cerb::debug
