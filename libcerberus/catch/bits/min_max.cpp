#include <cerberus/debug/debug.hpp>
#include <cerberus/number.hpp>

namespace cerb::debug
{
    CERBERUS_TEST_FUNC(testMinMaxOnInts)
    {
        ASSERT_EQUAL(min(10, -41, 0, 14515), -41);
        ASSERT_EQUAL(max(10, -41, 0, 14515), 14515);
        return true;
    }

    CERBERUS_TEST_FUNC(testMinMaxOnFloats)
    {
        // NOLINTBEGIN
        ASSERT_EQUAL(min(10.1551, -41.51961, 14515.51616, -5151.5161), -5151.5161);
        ASSERT_EQUAL(max(10.1551, -41.51961, 14515.51616, -5151.5161), 14515.51616);
        // NOLINTEND
        return true;
    }

    auto testMinMax() -> int
    {
        CERBERUS_TEST(testMinMaxOnInts());
        CERBERUS_TEST(testMinMaxOnFloats());
        return 0;
    }
}// namespace cerb::debug
