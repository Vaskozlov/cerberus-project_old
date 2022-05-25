#include <cerberus/debug/debug.hpp>
#include <cerberus/number.hpp>

namespace cerb::debug
{
    template<typename To, typename From>
    CERBLIB_DECL auto cast(From value) -> To
    {
        return static_cast<To>(value);
    }

    template<std::unsigned_integral UInt>
    CERBLIB_DECL auto pow2f(UInt power) -> float
    {
        return cast<float>(pow2<u32>(power));
    }

    template<std::unsigned_integral UInt>
    CERBLIB_DECL auto pow2d(UInt power) -> double
    {
        return cast<double>(pow2<u32>(power));
    }

    CERBLIB_DECL auto testLog2OnInts() -> bool
    {
        EXPECT_EQUAL(log2(pow2<u32>(3U)), 3);
        EXPECT_EQUAL(log2(pow2<u32>(0U)), 0);
        EXPECT_EQUAL(log2(pow2<u64>(31UL)), 31);
        EXPECT_EQUAL(log2(0U), -1);
        return true;
    }

    CERBLIB_DECL auto testLog2OnF32() -> bool
    {
        EXPECT_EQUAL(log2(pow2f(3U)), 3);
        EXPECT_EQUAL(log2(pow2f(0U)), 0);
        EXPECT_EQUAL(log2(pow2f(10U)), 10);
        EXPECT_EQUAL(log2(0.0F), -1);
        return true;
    }

    CERBLIB_DECL auto testLog2OnF64() -> bool
    {
        EXPECT_EQUAL(log2(pow2d(3U)), 3);
        EXPECT_EQUAL(log2(pow2d(0U)), 0);
        EXPECT_EQUAL(log2(pow2d(10U)), 10);
        EXPECT_EQUAL(log2(0.0), -1);
        return true;
    }

    auto testLog2() -> int
    {
        CERBERUS_TEST(testLog2OnInts());
        CERBERUS_TEST(testLog2OnF32());
        CERBERUS_TEST(testLog2OnF64());
        return 0;
    }
}// namespace cerb::debug
