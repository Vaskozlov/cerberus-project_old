#include <cerberus/debug/debug.hpp>
#include <cerberus/range.hpp>
#include <cerberus/string_view.hpp>
#include <string>

namespace cerb::debug
{
    CERBLIB_DECL auto testEqualStringViewComparisonWithStdStringView() -> bool
    {
        std::string_view standard_str_view = "Hello, World!";
        cerb::string_view str_view = "Hello, World!";

        EXPECT_TRUE(str_view == standard_str_view);
        EXPECT_FALSE(str_view != standard_str_view);
        EXPECT_TRUE(str_view >= standard_str_view);
        EXPECT_TRUE(str_view <= standard_str_view);
        EXPECT_FALSE(str_view > standard_str_view);
        EXPECT_FALSE(str_view < standard_str_view);

        EXPECT_TRUE(str_view.strView() == standard_str_view);

        EXPECT_TRUE(str_view.back() == standard_str_view.back());
        EXPECT_TRUE(str_view.front() == standard_str_view.front());
        EXPECT_TRUE(str_view.at(2) == standard_str_view.at(2));
        EXPECT_TRUE(str_view.find('!') == standard_str_view.find('!'));

        EXPECT_TRUE(std::ranges::equal(standard_str_view, str_view));

        return true;
    }

    CERBLIB_DECL auto testNotEqualStringViewComparisonWithStdStringView() -> bool
    {
        std::string_view standard_str_view = "It's a \"Hello, World!\" program";
        cerb::string_view str_view = "It's not a \"Hello, World!\" program";

        EXPECT_FALSE(str_view == standard_str_view);
        EXPECT_TRUE(str_view != standard_str_view);
        EXPECT_TRUE(str_view >= standard_str_view);
        EXPECT_FALSE(str_view <= standard_str_view);
        EXPECT_TRUE(str_view > standard_str_view);
        EXPECT_FALSE(str_view < standard_str_view);

        EXPECT_TRUE(str_view.back() == standard_str_view.back());
        EXPECT_TRUE(str_view.front() == standard_str_view.front());
        EXPECT_TRUE(str_view.find('!') != standard_str_view.find('!'));

        return true;
    }

    CERBLIB_DECL auto testStringViewContainsAt() -> bool
    {
        cerb::string_view str = "Hello, World!?";
        EXPECT_TRUE(str.containsAt(0, "Hello"));
        EXPECT_TRUE(str.containsAt(7, "World!"));
        EXPECT_FALSE(str.containsAt(7, "World!!"));
        EXPECT_FALSE(str.containsAt(0, "World!"));

        return true;
    }

    auto testStringView() -> int
    {
        CERBERUS_TEST(testEqualStringViewComparisonWithStdStringView());
        CERBERUS_TEST(testNotEqualStringViewComparisonWithStdStringView());
        CERBERUS_TEST(testStringViewContainsAt());
        return 0;
    }
}// namespace cerb::debug