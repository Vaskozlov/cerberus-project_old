#include <cerberus/debug/debug.hpp>
#include <cerberus/number.hpp>

namespace cerb::debug
{
    CERBLIB_DECL auto testBitScanForward() -> bool
    {
        EXPECT_TRUE(bit::scanForward<1>(0b1000U) == 3);
        EXPECT_TRUE(bit::scanForward<1>(0b1001U) == 0);
        return true;
    }

    CERBLIB_DECL auto testBitScanReverse() -> bool
    {
        EXPECT_TRUE(bit::scanReverse<1>(0b1000U) == 3);
        EXPECT_TRUE(bit::scanReverse<1>(0b1001U) == 3);
        return true;
    }

    auto testBitScan() -> int
    {
        CR_EXPECT_TRUE(testBitScanForward());
        CR_EXPECT_TRUE(testBitScanReverse());

        return 0;
    }
}// namespace cerb::debug
