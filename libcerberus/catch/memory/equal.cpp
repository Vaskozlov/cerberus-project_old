#include <cerberus/debug/debug.hpp>
#include <cerberus/memory.hpp>
#include <string_view>

namespace cerb::debug
{
    CERBERUS_TEST_FUNC(testEqualOnArrayOfInts)
    {
        std::array first_array = { 10, 20, 30, 40, 50 };
        std::array second_array = { 10, 20, 30, 40, 51 };

        ASSERT_TRUE(equal(first_array, first_array));
        ASSERT_FALSE(equal(first_array, second_array));

        return true;
    }

    CERBERUS_TEST_FUNC(testEqualOnString)
    {
        std::string_view first_string = "Hello, World?";
        std::string_view second_string = "Hello, World!";
        std::string_view copy_of_first_string = "Hello, World?";

        ASSERT_FALSE(equal(first_string, second_string));
        ASSERT_TRUE(equal(first_string, copy_of_first_string));

        return true;
    }

    auto testEqual() -> int
    {
        CERBERUS_TEST(testEqualOnArrayOfInts());
        CERBERUS_TEST(testEqualOnString());
        return 0;
    }
}// namespace cerb::debug
