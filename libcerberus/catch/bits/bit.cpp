#include <cerberus/debug/debug.hpp>

namespace cerb::debug
{
    auto testAbs() -> int;
    auto testPow2() -> int;
    auto testLog2() -> int;
    auto testMinMax() -> int;
    auto testBitmap() -> int;
    auto testBitScan() -> int;

    auto testByteMask() -> void
    {
        ByteMask mask{ 0ll };
        mask.mask_8[0] = 255;
        mask.mask_8[2] = 255;

        EXPECT_TRUE(mask.getAsInt() == 0xFF00FF);

        mask.getAsInt() += 0xFF00;

        EXPECT_TRUE(mask.getAsInt() == 0xFFFFFF);
    }

    auto testBit() -> int
    {
        testAbs();
        testPow2();
        testLog2();
        testMinMax();
        testByteMask();
        testBitScan();
        return 0;
    }
}// namespace cerb::debug
