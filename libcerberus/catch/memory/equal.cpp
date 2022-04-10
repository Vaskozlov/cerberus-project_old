#include <cerberus/debug/debug.hpp>
#include <cerberus/memory.hpp>
#include <string_view>

namespace cerb::debug
{
    CERBLIB_DECL auto testEqualOnArrayOfInts() -> bool
    {
        std::array first_array = { 10, 20, 30, 40, 50 };
        std::array second_array = { 10, 20, 30, 40, 51 };

        EXPECT_TRUE(equal(first_array, first_array));
        EXPECT_FALSE(equal(first_array, second_array));

        return true;
    }

    CERBLIB_DECL auto testEqualOnString() -> bool
    {
        using namespace std::string_view_literals;

        auto first_string = "Hello, World?"sv;
        auto second_string = "Hello, World!"sv;
        auto copy_of_first_string = "Hello, World?"sv;

        EXPECT_FALSE(equal(first_string, second_string));
        EXPECT_TRUE(equal(first_string, copy_of_first_string));

        return true;
    }

    auto testEqual() -> int
    {
        CERBERUS_TEST(testEqualOnArrayOfInts());
        CERBERUS_TEST(testEqualOnString());
        return 0;
    }
}// namespace cerb::debug
