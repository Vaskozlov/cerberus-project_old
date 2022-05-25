#include <cerberus/debug/debug.hpp>
#include <cerberus/format/format.hpp>
#include <fmt/format.h>
#include <set>
#include <vector>

namespace cerb::debug
{
    using namespace string_view_literals;

    constexpr static string_view TestCharStringView = "Hello, World!"_sv;
    constexpr static u16string_view TestChar16StringView = u"Hello, World!"_sv;
    constexpr static std::array<int, 4> TestArrayOfInts = { 10, 20, 30, 40 };

    CERBERUS_TEST_FUNC_STD_STRING(testFmtOnBasicChar)
    {
        EXPECT_EQUAL(fmt::format<char>("Hello, World!"_sv), "Hello, World!");

        EXPECT_EQUAL(fmt::format<char>("Hello, World! {}!"_sv, 10), "Hello, World! 10!");

        EXPECT_EQUAL(
            fmt::format<char>("Hello, World! {}!"_sv, TestArrayOfInts),
            "Hello, World! [10, 20, 30, 40]!");

        EXPECT_EQUAL(
            fmt::format<char>("Hello, World! {} {} {}"_sv, 10, TestArrayOfInts, TestCharStringView),
            "Hello, World! 10 [10, 20, 30, 40] Hello, World!");

        return true;
    }

    CERBERUS_TEST_FUNC_STD_STRING(testFmtOnUtf16Char)
    {
        EXPECT_EQUAL(fmt::format<char16_t>("Hello, World!"_sv), u"Hello, World!");

        EXPECT_EQUAL(fmt::format<char16_t>("Hello, World! {}!"_sv, 10), u"Hello, World! 10!");

        EXPECT_EQUAL(
            fmt::format<char16_t>("Hello, World! {}!"_sv, TestChar16StringView),
            u"Hello, "
            "World! Hello, World!!");

        EXPECT_EQUAL(
            fmt::format<char16_t>("Hello, World! {}!"_sv, TestArrayOfInts),
            u"Hello, World! [10, 20, 30, 40]!");

        EXPECT_EQUAL(
            fmt::format<char16_t>(
                "Hello, World! {} {} {}"_sv, 10, TestArrayOfInts, TestCharStringView),
            u"Hello, World! 10 [10, 20, 30, 40] Hello, World!");

        return true;
    }

    auto testFmt() -> int
    {
        CERBERUS_TEST_STD_STRING(testFmtOnBasicChar());
        CERBERUS_TEST_STD_STRING(testFmtOnUtf16Char());
        return 0;
    }
}// namespace cerb::debug
