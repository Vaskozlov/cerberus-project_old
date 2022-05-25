#include <cerberus/debug/debug.hpp>

namespace cerb::debug
{
    auto testByteMask() -> void
    {
        // NOLINTBEGIN
        ByteMask mask{ 0LL };
        mask.mask_8[0] = 0xFF;
        mask.mask_8[2] = 0xFF;

        EXPECT_EQUAL(mask.getAsInt(), 0xFF00FF);

        mask.getAsInt() += 0xFF00;

        EXPECT_EQUAL(mask.getAsInt(), 0xFFFFFF);
        // NOLINTEND
    }
}// namespace cerb::debug
