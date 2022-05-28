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

        ASSERT_TRUE(str_view == standard_str_view);
        ASSERT_TRUE(str_view >= standard_str_view);
        ASSERT_TRUE(str_view <= standard_str_view);

        ASSERT_FALSE(str_view != standard_str_view);
        ASSERT_FALSE(str_view > standard_str_view);
        ASSERT_FALSE(str_view < standard_str_view);

        return true;
    }

    CERBERUS_TEST_FUNC(testStringViewAt)
    {
        std::string_view standard_str_view = "Hello, World!";
        cerb::string_view str_view = "Hello, World!";

        ASSERT_TRUE(str_view == standard_str_view);

        for (size_t i = 0; i != str_view.size(); ++i) {
            ASSERT_EQUAL(str_view.at(i), standard_str_view.at(i));
        }

        return true;
    }


    CERBERUS_TEST_FUNC(testStringViewConvertionToStdStringView)
    {
        std::string_view standard_str_view = "Hello, World!";
        cerb::string_view str_view = "Hello, World!";

        ASSERT_EQUAL(str_view.strView(), standard_str_view);

        return true;
    }

    CERBERUS_TEST_FUNC(testStringViewComparisonWithStdStringViewByStdRangesEqual)
    {
        std::string_view standard_str_view = "Hello, World!";
        cerb::string_view str_view = "Hello, World!";

        ASSERT_TRUE(std::ranges::equal(standard_str_view, str_view));

        return true;
    }

    CERBERUS_TEST_FUNC(testStringViewBackAndFront)
    {
        std::string_view standard_str_view = "It's a \"Hello, World!\" program";
        cerb::string_view str_view = "It's not a \"Hello, World!\" program";

        ASSERT_TRUE(str_view.back() == standard_str_view.back());
        ASSERT_TRUE(str_view.front() == standard_str_view.front());

        return true;
    }

    CERBERUS_TEST_FUNC(testNotEqualStringViewComparisonWithStdStringView)
    {
        std::string_view standard_str_view = "It's a \"Hello, World!\" program";
        cerb::string_view str_view = "It's not a \"Hello, World!\" program";

        ASSERT_FALSE(str_view == standard_str_view);
        ASSERT_FALSE(str_view <= standard_str_view);
        ASSERT_FALSE(str_view < standard_str_view);

        ASSERT_TRUE(str_view != standard_str_view);
        ASSERT_TRUE(str_view >= standard_str_view);
        ASSERT_TRUE(str_view > standard_str_view);

        return true;
    }

    CERBERUS_TEST_FUNC(testStringViewContainsAt)
    {
        cerb::string_view str = "Hello, World!?";

        ASSERT_TRUE(str.containsAt(0, "Hello"));
        ASSERT_TRUE(str.containsAt(7, "World!"));

        ASSERT_FALSE(str.containsAt(7, "World!!"));
        ASSERT_FALSE(str.containsAt(0, "World!"));

        return true;
    }

    CERBERUS_TEST_FUNC(testStringViewFind)
    {
        std::string_view standard_str_view = "It's a \"Hello, World!\" program";
        cerb::string_view str_view = "It's a \"Hello, World!\" program";

        ASSERT_EQUAL(str_view.find('I'), standard_str_view.find('I'));
        ASSERT_EQUAL(str_view.find('m'), standard_str_view.find('m'));
        ASSERT_EQUAL(str_view.find('\''), standard_str_view.find('\''));

        ASSERT_EQUAL(str_view.find('!'), standard_str_view.find('!'));
        ASSERT_EQUAL(str_view.find('\0'), standard_str_view.find('\0'));

        return true;
    }

    CERBERUS_TEST_FUNC(testStringViewRfind)
    {
        std::string_view std_str = "Hello, World!?";
        cerb::string_view str = "Hello, World!?";

        ASSERT_EQUAL(str.rfind('?'), std_str.rfind('?'));
        ASSERT_EQUAL(str.rfind('!'), std_str.rfind('!'));
        ASSERT_EQUAL(str.rfind('H'), std_str.rfind('H'));
        ASSERT_EQUAL(str.rfind('\0'), std_str.rfind('\0'));
        ASSERT_EQUAL(str.rfind('\1'), std_str.rfind('\1'));

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