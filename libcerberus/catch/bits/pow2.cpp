#include <cerberus/debug/debug.hpp>
#include <cerberus/number.hpp>

namespace cerb::debug
{
    CERBERUS_TEST_FUNC(testPow2OnInt)
    {
        ASSERT_EQUAL(pow2<u32>(0U), 1U);
        ASSERT_EQUAL(pow2<u32>(2U), 4U);
        ASSERT_EQUAL(pow2<u32>(10U), 1024U);
        ASSERT_EQUAL(pow2<u64>(54ULL), (1ULL << 54ULL));
        return true;
    }

    CERBERUS_TEST_FUNC(testPow2OnF32)
    {
        // NOLINTBEGIN
        ASSERT_EQUAL(pow2<f32>(0U), 1.0F);
        ASSERT_EQUAL(pow2<f32>(2U), 4.0F);
        ASSERT_EQUAL(pow2<f32>(10U), 1024.0F);
        ASSERT_EQUAL(pow2<f32>(54ULL), 18014398509481984ULL);
        // NOLINTEND
        return true;
    }

    CERBERUS_TEST_FUNC(testPow2OnF64)
    {
        // NOLINTBEGIN
        ASSERT_EQUAL(pow2<f64>(0U), 1.0);
        ASSERT_EQUAL(pow2<f64>(2U), 4.0);
        ASSERT_EQUAL(pow2<f64>(10U), 1024.0);
        ASSERT_EQUAL(pow2<f64>(54ULL), 18014398509481984ULL);
        // NOLINTEND
        return true;
    }

    auto testPow2() -> int
    {
        CERBERUS_TEST(testPow2OnInt());
        CERBERUS_TEST(testPow2OnF32());
        CERBERUS_TEST(testPow2OnF64());
        return 0;
    }
}// namespace cerb::debug
