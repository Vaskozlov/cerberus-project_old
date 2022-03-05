#include "log2.hpp"
#include <cerberus/debug/debug.hpp>
#include <cerberus/number.hpp>

namespace cerb::debug
{
    CERBLIB_DECL auto testLog2OnInts() -> bool
    {
        EXPECT_TRUE(cerb::log2(pow2<u32>(3U)) == 3);
        EXPECT_TRUE(cerb::log2(pow2<u32>(0U)) == 0);
        EXPECT_TRUE(cerb::log2(pow2<u64>(31UL)) == 31);
        EXPECT_TRUE(cerb::log2(0U) == bitsizeof(size_t));
        return true;
    }

    CERBLIB_DECL auto testLog2OnF32() -> bool
    {
        EXPECT_TRUE(cerb::log2(8.0f) == 3);
        EXPECT_TRUE(cerb::log2(1.0f) == 0);
        EXPECT_TRUE(cerb::log2(1024.0f) == 10);
        EXPECT_TRUE(cerb::log2(0.0f) == -1);
        EXPECT_TRUE(cerb::log2(-10.0f) == -1);
        return true;
    }

    CERBLIB_DECL auto testLog2OnF64() -> bool
    {
        EXPECT_TRUE(cerb::log2(8.0) == 3);
        EXPECT_TRUE(cerb::log2(1.0) == 0);
        EXPECT_TRUE(cerb::log2(1024.0) == 10);
        EXPECT_TRUE(cerb::log2(0.0) == -1);
        EXPECT_TRUE(cerb::log2(-10.0) == -1);
        return true;
    }

    auto testLog2() -> int
    {
        CR_EXPECT_TRUE(testLog2OnInts());
        CR_EXPECT_TRUE(testLog2OnF32());
        CR_EXPECT_TRUE(testLog2OnF64());
        return 0;
    }
}// namespace cerb::debug
