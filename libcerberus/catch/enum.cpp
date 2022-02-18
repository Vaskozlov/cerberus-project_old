#include "enum.hpp"
#include <cerberus/enum.hpp>

// NOLINTNEXTLINE
CERBERUS_ENUM(TestEnum, cerb::u32, VALUE0 = 0b1, VALUE1 = 0b10, VALUE2 = 0b100);

namespace cerb::test
{
    auto enumTest(u32) -> int
    {
        TestEnum test(TestEnum::VALUE0);

        EXPECT_TRUE((test | TestEnum::VALUE1).get() == 0b11);
        EXPECT_TRUE((test & TestEnum::VALUE1).get() == 0);

        test |= TestEnum::VALUE1;
        EXPECT_TRUE(test.get() == 0b11);

        test &= TestEnum::VALUE2;
        EXPECT_TRUE(test.get() == 0b0);

        return 0;
    }
}// namespace cerb::test
