    #include <cerberus/debug/debug.hpp>

namespace cerb::debug
{
    auto testByteMask() -> void
    {
        ByteMask mask{ 0LL };
        mask.mask_8[0] = 255;
        mask.mask_8[2] = 255;

        EXPECT_TRUE(mask.getAsInt() == 0xFF00FF);

        mask.getAsInt() += 0xFF00;

        EXPECT_TRUE(mask.getAsInt() == 0xFFFFFF);
    }
}// namespace cerb::debug
