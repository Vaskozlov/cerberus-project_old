#include <cerberus/debug/debug.hpp>
#include <cerberus/number.hpp>
#include <cerberus/range.hpp>

namespace cerb::debug
{
    CERBERUS_TEST_FUNC(testBasicRange)
    {
        size_t control_sum = 0;
        std::array<uint, 10> expected_values = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

        for (uint i : Range(10U)) {
            control_sum += i;
            EXPECT_EQUAL(i, expected_values[i]);
        }

        EXPECT_EQUAL(control_sum, 45);
        return true;
    }

    CERBERUS_TEST_FUNC(testRangeWithDoubleBorder)
    {
        size_t control_sum = 0;
        std::array<uint, 10> expected_values = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };

        for (uint i : Range(10U, 20U)) {
            control_sum += i;
            EXPECT_EQUAL(i, expected_values[i - 10U]);
        }

        EXPECT_EQUAL(control_sum, 145);
        return true;
    }

    CERBERUS_TEST_FUNC(testRangeWithDoubleBorderAndInc)
    {
        size_t control_sum = 0;
        std::array<uint, 11> expected_values = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };

        for (uint i : Range(10U, 23U, 5U)) {
            control_sum += i;
            EXPECT_EQUAL(i, expected_values[i - 10U]);
        }

        EXPECT_EQUAL(control_sum, 45);
        return true;
    }

    CERBERUS_TEST_FUNC(testRangeWithDoubleBorderAndNegativeInc)
    {
        ssize_t control_sum = 0;
        std::array<int, 10> expected_values = { 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };

        for (int i : Range(20, 10, -1)) {
            control_sum += i;
            EXPECT_EQUAL(i, expected_values[asUInt(i - 11)]);
        }

        EXPECT_EQUAL(control_sum, 155);
        return true;
    }

    CERBERUS_TEST_FUNC(testRangeWithDoubleBorderAndNegativeInc5)
    {
        ssize_t control_sum = 0;
        std::array<int, 10> expected_values = { 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };

        for (int i : Range(20, 13, -5)) {
            control_sum += i;
            EXPECT_EQUAL(i, expected_values[asUInt(i - 11)]);
        }

        EXPECT_EQUAL(control_sum, 35);
        return true;
    }

    auto testRange() -> int
    {
        CERBERUS_TEST(testBasicRange());
        CERBERUS_TEST(testRangeWithDoubleBorder());
        CERBERUS_TEST(testRangeWithDoubleBorderAndInc());
        CERBERUS_TEST(testRangeWithDoubleBorderAndNegativeInc());
        CERBERUS_TEST(testRangeWithDoubleBorderAndNegativeInc5());
        return 0;
    }
}// namespace cerb::debug