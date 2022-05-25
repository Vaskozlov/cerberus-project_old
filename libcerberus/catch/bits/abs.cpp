#include <cerberus/debug/debug.hpp>
#include <cerberus/number.hpp>

namespace cerb::debug
{
    CERBERUS_TEST_FUNC(testAbsOnInts)
    {
        EXPECT_EQUAL(abs(0), 0);
        EXPECT_EQUAL(abs(10), 10);

        EXPECT_EQUAL(abs(-10), 10);
        EXPECT_EQUAL(abs(-10), 10U);

        return true;
    }

    CERBERUS_TEST_FUNC(testAbsOnFloats)
    {
        // NOLINTBEGIN
        EXPECT_EQUAL(abs(0.0F), 0.0F);
        EXPECT_EQUAL(abs(10.0F), 10.0);

        EXPECT_EQUAL(abs(-0.0F), 0.0F);
        EXPECT_EQUAL(abs(-10.0F), 10.0);

        EXPECT_EQUAL(abs(0.0F), 0.0F);
        EXPECT_EQUAL(abs(10.0F), 10.0);

        EXPECT_EQUAL(abs(-0.0), 0.0);
        EXPECT_EQUAL(abs(-10.0), 10.0);
        // NOLINTEND

        return true;
    }

    auto testAbs() -> int
    {
        CERBERUS_TEST(testAbsOnInts());
        CERBERUS_TEST(testAbsOnFloats());
        return 0;
    }
}// namespace cerb::debug
