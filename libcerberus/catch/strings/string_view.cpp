#include <cerberus/debug/debug.hpp>
#include <cerberus/range.hpp>
#include <cerberus/string_view.hpp>
#include <string>

namespace cerb::debug
{
    CERBERUS_TEST_FUNC(testEqualStringViewComparisonWithStdStringView)
    {
        std::string_view standard_str_view = "Hello, World!";
        cerb::string_view str_view = "Hello, World!";

        EXPECT_TRUE(str_view == standard_str_view);
        EXPECT_TRUE(str_view >= standard_str_view);
        EXPECT_TRUE(str_view <= standard_str_view);

        EXPECT_FALSE(str_view != standard_str_view);
        EXPECT_FALSE(str_view > standard_str_view);
        EXPECT_FALSE(str_view < standard_str_view);

        return true;
    }

    CERBERUS_TEST_FUNC(testStringViewAt)
    {
        std::string_view standard_str_view = "Hello, World!";
        cerb::string_view str_view = "Hello, World!";

        EXPECT_TRUE(str_view == standard_str_view);

        for (size_t i = 0; i != str_view.size(); ++i) {
            EXPECT_EQUAL(str_view.at(i), standard_str_view.at(i));
        }

        return true;
    }


    CERBERUS_TEST_FUNC(testStringViewConvertionToStdStringView)
    {
        std::string_view standard_str_view = "Hello, World!";
        cerb::string_view str_view = "Hello, World!";

        EXPECT_EQUAL(str_view.strView(), standard_str_view);

        return true;
    }

    CERBERUS_TEST_FUNC(testStringViewComparisonWithStdStringViewByStdRangesEqual)
    {
        std::string_view standard_str_view = "Hello, World!";
        cerb::string_view str_view = "Hello, World!";

        EXPECT_TRUE(std::ranges::equal(standard_str_view, str_view));

        return true;
    }

    CERBERUS_TEST_FUNC(testStringViewBackAndFront)
    {
        std::string_view standard_str_view = "It's a \"Hello, World!\" program";
        cerb::string_view str_view = "It's not a \"Hello, World!\" program";

        EXPECT_TRUE(str_view.back() == standard_str_view.back());
        EXPECT_TRUE(str_view.front() == standard_str_view.front());

        return true;
    }

    CERBERUS_TEST_FUNC(testNotEqualStringViewComparisonWithStdStringView)
    {
        std::string_view standard_str_view = "It's a \"Hello, World!\" program";
        cerb::string_view str_view = "It's not a \"Hello, World!\" program";

        EXPECT_FALSE(str_view == standard_str_view);
        EXPECT_FALSE(str_view <= standard_str_view);
        EXPECT_FALSE(str_view < standard_str_view);

        EXPECT_TRUE(str_view != standard_str_view);
        EXPECT_TRUE(str_view >= standard_str_view);
        EXPECT_TRUE(str_view > standard_str_view);

        return true;
    }

    CERBERUS_TEST_FUNC(testStringViewContainsAt)
    {
        cerb::string_view str = "Hello, World!?";

        EXPECT_TRUE(str.containsAt(0, "Hello"));
        EXPECT_TRUE(str.containsAt(7, "World!"));

        EXPECT_FALSE(str.containsAt(7, "World!!"));
        EXPECT_FALSE(str.containsAt(0, "World!"));

        return true;
    }

    CERBERUS_TEST_FUNC(testStringViewFind)
    {
        std::string_view standard_str_view = "It's a \"Hello, World!\" program";
        cerb::string_view str_view = "It's a \"Hello, World!\" program";

        EXPECT_EQUAL(str_view.find('I'), standard_str_view.find('I'));
        EXPECT_EQUAL(str_view.find('m'), standard_str_view.find('m'));
        EXPECT_EQUAL(str_view.find('\''), standard_str_view.find('\''));

        EXPECT_EQUAL(str_view.find('!'), standard_str_view.find('!'));
        EXPECT_EQUAL(str_view.find('\0'), standard_str_view.find('\0'));

        return true;
    }

    CERBERUS_TEST_FUNC(testStringViewRfind)
    {
        std::string_view std_str = "Hello, World!?";
        cerb::string_view str = "Hello, World!?";

        EXPECT_EQUAL(str.rfind('?'), std_str.rfind('?'));
        EXPECT_EQUAL(str.rfind('!'), std_str.rfind('!'));
        EXPECT_EQUAL(str.rfind('H'), std_str.rfind('H'));
        EXPECT_EQUAL(str.rfind('\0'), std_str.rfind('\0'));
        EXPECT_EQUAL(str.rfind('\1'), std_str.rfind('\1'));

        return true;
    }

    auto testStringView() -> int
    {
        CERBERUS_TEST(testEqualStringViewComparisonWithStdStringView());
        CERBERUS_TEST(testStringViewAt());
        CERBERUS_TEST(testStringViewConvertionToStdStringView());
        CERBERUS_TEST(testStringViewComparisonWithStdStringViewByStdRangesEqual());
        CERBERUS_TEST(testStringViewBackAndFront());
        CERBERUS_TEST(testNotEqualStringViewComparisonWithStdStringView());
        CERBERUS_TEST(testStringViewFind());
        CERBERUS_TEST(testStringViewRfind());

        return 0;
    }
}// namespace cerb::debug