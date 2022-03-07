#include "bit.hpp"
#include "abs.hpp"
#include "bit_scan.hpp"
#include "log2.hpp"
#include "min_max.hpp"
#include "pow2.hpp"
#include <cerberus/number.hpp>

namespace cerb::debug
{
    auto testByteMask() -> void
    {
        ByteMask mask{ 0ll };
        mask.mask_8[0] = 255;
        mask.mask_8[2] = 255;

        EXPECT_TRUE(mask.getAsInt() == 0xFF00FF);

        mask.getAsInt() += 0xFF00;

        EXPECT_TRUE(mask.getAsInt() == 0xFFFFFF);
    }

    auto testBit(u32) -> int
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
