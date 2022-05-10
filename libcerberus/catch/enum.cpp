#include <cerberus/debug/debug.hpp>
#include <cerberus/enum.hpp>

// NOLINTNEXTLINE
CERBERUS_ENUM(TestEnum, cerb::u32, VALUE0 = 0b1, VALUE1 = 0b10, VALUE2 = 0b100);

namespace cerb::debug
{
    constexpr auto testEnumInitialization() -> bool
    {
        TestEnum test(TestEnum::VALUE0);
        return test.get() == 0b1;
    }

    constexpr auto testEnumIsSet() -> bool
    {
        TestEnum test(TestEnum::VALUE0);

        EXPECT_TRUE(test.isSet(TestEnum::VALUE0));
        EXPECT_FALSE(test.isSet(TestEnum::VALUE1));
        EXPECT_FALSE(test.isSet(TestEnum::VALUE2));

        return true;
    }

    constexpr auto testEnumOrOperator() -> bool
    {
        TestEnum test(TestEnum::VALUE0);

        test |= TestEnum::VALUE1;
        EXPECT_TRUE(test.isSet(TestEnum::VALUE0) && test.isSet(TestEnum::VALUE1));

        test = test | TestEnum::VALUE2;
        EXPECT_TRUE(
            test.isSet(TestEnum::VALUE0) && test.isSet(TestEnum::VALUE1) &&
            test.isSet(TestEnum::VALUE2));

        return true;
    }

    constexpr auto testEnumAndOperator() -> bool
    {
        TestEnum test(TestEnum::VALUE0 | TestEnum::VALUE1 | TestEnum::VALUE2);

        test &= TestEnum::VALUE0 | TestEnum::VALUE1;
        EXPECT_TRUE(test.isSet(TestEnum::VALUE0) && test.isSet(TestEnum::VALUE1));
        EXPECT_FALSE(test.isSet(TestEnum::VALUE2));

        test = test & TestEnum::VALUE0;
        EXPECT_TRUE(test.isSet(TestEnum::VALUE0));
        EXPECT_FALSE(test.isSet(TestEnum::VALUE2) || test.isSet(TestEnum::VALUE1));

        return true;
    }

    constexpr auto testEnumAnySet() -> bool
    {
        TestEnum test(TestEnum::VALUE0 | TestEnum::VALUE1);

        EXPECT_TRUE(test.isAnyOfSet(TestEnum::VALUE0 | TestEnum::VALUE2));
        EXPECT_TRUE(test.isAnyOfSet(TestEnum::VALUE0 | TestEnum::VALUE1));
        EXPECT_TRUE(test.isAnyOfSet(TestEnum::VALUE1 | TestEnum::VALUE2));

        EXPECT_FALSE(test.isAnyOfSet(TestEnum::VALUE2));

        return true;
    }

    auto testEnum() -> int
    {
        CERBERUS_TEST(testEnumInitialization());
        CERBERUS_TEST(testEnumIsSet());
        CERBERUS_TEST(testEnumOrOperator());
        CERBERUS_TEST(testEnumAndOperator());
        CERBERUS_TEST(testEnumAnySet());
        return 0;
    }
}// namespace cerb::debug
