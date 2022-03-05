#include "bit.hpp"
#include "abs.hpp"
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

    constexpr auto testBitScan() -> bool
    {
        EXPECT_TRUE(bit::scanForward<1>(0b1000U) == 3);
        EXPECT_TRUE(bit::scanForward<1>(0b1001U) == 0);
        EXPECT_TRUE(bit::scanReverse<1>(0b1000U) == 3);
        EXPECT_TRUE(bit::scanReverse<1>(0b1001U) == 3);

        return true;
    }

    auto testBit(u32) -> int
    {
        testAbs();
        testPow2();
        testLog2();
        testMinMax();
        testByteMask();
        CR_EXPECT_TRUE(testBitScan());
        return 0;
    }
}// namespace cerb::debug
