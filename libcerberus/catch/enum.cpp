#include <cerberus/debug/debug.hpp>
#include <cerberus/enum.hpp>

// NOLINTNEXTLINE
CERBERUS_ENUM(TestEnum, cerb::u32, VALUE0 = 0b1, VALUE1 = 0b10, VALUE2 = 0b100);

namespace cerb::debug
{
    CERBERUS_TEST_FUNC(testEnumInitialization)
    {
        TestEnum test(TestEnum::VALUE0);
        ASSERT_EQUAL(test.get(), 0b1);

        return true;
    }

    CERBERUS_TEST_FUNC(testEnumIsSet)
    {
        TestEnum test(TestEnum::VALUE0);

        ASSERT_TRUE(test.isSet(TestEnum::VALUE0));

        ASSERT_FALSE(test.isSet(TestEnum::VALUE1));
        ASSERT_FALSE(test.isSet(TestEnum::VALUE2));

        return true;
    }

    CERBERUS_TEST_FUNC(testEnumOrOperator)
    {
        TestEnum test(TestEnum::VALUE0);

        test |= TestEnum::VALUE1;
        ASSERT_TRUE(test.isSet(TestEnum::VALUE0) && test.isSet(TestEnum::VALUE1));

        test = test | TestEnum::VALUE2;
        ASSERT_TRUE(
            test.isSet(TestEnum::VALUE0) && test.isSet(TestEnum::VALUE1) &&
            test.isSet(TestEnum::VALUE2));

        return true;
    }

    CERBERUS_TEST_FUNC(testEnumAndOperator)
    {
        TestEnum test(TestEnum::VALUE0 | TestEnum::VALUE1 | TestEnum::VALUE2);

        test &= TestEnum::VALUE0 | TestEnum::VALUE1;
        ASSERT_TRUE(test.isSet(TestEnum::VALUE0) && test.isSet(TestEnum::VALUE1));
        ASSERT_FALSE(test.isSet(TestEnum::VALUE2));

        test = test & TestEnum::VALUE0;
        ASSERT_TRUE(test.isSet(TestEnum::VALUE0));
        ASSERT_FALSE(test.isSet(TestEnum::VALUE2) || test.isSet(TestEnum::VALUE1));

        return true;
    }

    CERBERUS_TEST_FUNC(testEnumAnySet)
    {
        TestEnum test(TestEnum::VALUE0 | TestEnum::VALUE1);

        ASSERT_TRUE(test.isAnyOfSet(TestEnum::VALUE0 | TestEnum::VALUE2));
        ASSERT_TRUE(test.isAnyOfSet(TestEnum::VALUE0 | TestEnum::VALUE1));
        ASSERT_TRUE(test.isAnyOfSet(TestEnum::VALUE1 | TestEnum::VALUE2));

        ASSERT_FALSE(test.isAnyOfSet(TestEnum::VALUE2));

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
