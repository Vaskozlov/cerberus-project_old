#include <cerberus/debug/debug.hpp>
#include <cerberus/format/format.hpp>

namespace cerb::debug
{
    constexpr static string_view TestCharStringView = "Hello, World!";
    constexpr static u16string_view TestChar16StringView = u"Hello, World!";
    constexpr static std::array<int, 4> TestArrayOfInts = { 10, 20, 30, 40 };

    CERBERUS_TEST_FUNC_WITH_CONSTEXPR_STRING(testFormattingBasicChar)
    {
        ASSERT_EQUAL(fmt::format<char>("Hello, World!"), "Hello, World!");
        return true;
    }

    CERBERUS_TEST_FUNC_WITH_CONSTEXPR_STRING(testFormattingOnIntBasicChar)
    {
        ASSERT_EQUAL(fmt::format<char>("{}", 10), "10");
        return true;
    }

    CERBERUS_TEST_FUNC_WITH_CONSTEXPR_STRING(testFormattingOnPairBasicChar)
    {
        ASSERT_EQUAL(fmt::format<char>("{}", Pair{ 0, 10 }), "{0, 10}");
        return true;
    }

    CERBERUS_TEST_FUNC_WITH_CONSTEXPR_STRING(testFormattingOnArrayBasicChar)
    {
        ASSERT_EQUAL(fmt::format<char>("{}", TestArrayOfInts), "[10, 20, 30, 40]");
        return true;
    }

    CERBERUS_TEST_FUNC_WITH_CONSTEXPR_STRING(testFormattingOnStringBasicChar)
    {
        ASSERT_EQUAL(fmt::format<char>("{}", TestCharStringView), "Hello, World!");
        return true;
    }

    CERBERUS_TEST_FUNC_WITH_CONSTEXPR_STRING(testFormattingMultiArgumentsBasicChar)
    {
        ASSERT_EQUAL(
            fmt::format<char>("Hello, World! {} {} {}", 10, TestArrayOfInts, TestCharStringView),
            "Hello, World! 10 [10, 20, 30, 40] Hello, World!");
        return true;
    }

    CERBERUS_TEST_FUNC_WITH_CONSTEXPR_STRING(testFormattingUtf16Char)
    {
        ASSERT_EQUAL(fmt::format<char16_t>("Hello, World!"), u"Hello, World!");
        return true;
    }

    CERBERUS_TEST_FUNC_WITH_CONSTEXPR_STRING(testFormattingOnIntUtf16Char)
    {
        ASSERT_EQUAL(fmt::format<char16_t>("{}", 10), u"10");
        return true;
    }

    CERBERUS_TEST_FUNC_WITH_CONSTEXPR_STRING(testFormattingOnPairUtf16Char)
    {
        ASSERT_EQUAL(fmt::format<char16_t>("{}", Pair{ 0, 10 }), u"{0, 10}");
        return true;
    }

    CERBERUS_TEST_FUNC_WITH_CONSTEXPR_STRING(testFormattingOnArrayUtf16Char)
    {
        ASSERT_EQUAL(fmt::format<char16_t>("{}", TestArrayOfInts), u"[10, 20, 30, 40]");
        return true;
    }

    CERBERUS_TEST_FUNC_WITH_CONSTEXPR_STRING(testFormattingOnStringUtf16Char)
    {
        ASSERT_EQUAL(fmt::format<char16_t>("{}", TestCharStringView), u"Hello, World!");
        return true;
    }

    CERBERUS_TEST_FUNC_WITH_CONSTEXPR_STRING(testFormattingMultiArgumentsUtf16Char)
    {
        ASSERT_EQUAL(
            fmt::format<char16_t>(
                "Hello, World! {} {} {}", 10, TestArrayOfInts, TestChar16StringView),
            u"Hello, World! 10 [10, 20, 30, 40] Hello, World!");
        return true;
    }

    auto testFmt() -> int
    {
        CERBERUS_TEST_STD_STRING(testFormattingBasicChar());
        CERBERUS_TEST_STD_STRING(testFormattingOnIntBasicChar());
        CERBERUS_TEST_STD_STRING(testFormattingOnPairBasicChar());
        CERBERUS_TEST_STD_STRING(testFormattingOnArrayBasicChar());
        CERBERUS_TEST_STD_STRING(testFormattingOnStringBasicChar());
        CERBERUS_TEST_STD_STRING(testFormattingMultiArgumentsBasicChar());

        CERBERUS_TEST_STD_STRING(testFormattingUtf16Char());
        CERBERUS_TEST_STD_STRING(testFormattingOnIntUtf16Char());
        CERBERUS_TEST_STD_STRING(testFormattingOnPairUtf16Char());
        CERBERUS_TEST_STD_STRING(testFormattingOnArrayUtf16Char());
        CERBERUS_TEST_STD_STRING(testFormattingOnStringUtf16Char());
        CERBERUS_TEST_STD_STRING(testFormattingMultiArgumentsUtf16Char());

        return 0;
    }
}// namespace cerb::debug
