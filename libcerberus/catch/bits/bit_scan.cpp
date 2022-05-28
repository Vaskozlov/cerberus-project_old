#include <cerberus/debug/debug.hpp>
#include <cerberus/number.hpp>

namespace cerb::debug
{
    CERBERUS_TEST_FUNC(testBitScanForward)
    {
        ASSERT_EQUAL(bit::scanForward<1>(0b1000U), 3);
        ASSERT_EQUAL(bit::scanForward<1>(0b1001U), 0);
        return true;
    }

    CERBERUS_TEST_FUNC(testBitScanReverse)
    {
        ASSERT_EQUAL(bit::scanReverse<1>(0b1000U), 3);
        ASSERT_EQUAL(bit::scanReverse<1>(0b1001U), 3);
        return true;
    }

    auto testBitScan() -> int
    {
        CERBERUS_TEST(testBitScanForward());
        CERBERUS_TEST(testBitScanReverse());

        return 0;
    }
}// namespace cerb::debug
