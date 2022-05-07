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
    const static std::set<int> TestSetOfInts = { 10, 20, 30, 40 };
    const static std::vector<int> TestVectorOfInts = { 10, 20, 30, 40 };

    auto testFmtOnBasicChar() -> void
    {
        EXPECT_TRUE(fmt::format<char>("Hello, World!"_sv) == "Hello, World!");

        EXPECT_TRUE(fmt::format<char>("Hello, World! {}!"_sv, 10) == "Hello, World! 10!");

        EXPECT_TRUE(
            fmt::format<char>("Hello, World! {}!"_sv, TestSetOfInts) ==
            "Hello, World! {10, 20, 30, 40}!");

        EXPECT_TRUE(
            fmt::format<char>("Hello, World! {}!"_sv, TestVectorOfInts) ==
            "Hello, World! [10, 20, 30, 40]!");

        EXPECT_TRUE(
            fmt::format<char>(
                "Hello, World! {} {} {} {}"_sv, 10, TestSetOfInts, TestVectorOfInts,
                TestCharStringView) ==
            "Hello, World! 10 {10, 20, 30, 40} [10, 20, 30, 40] Hello, World!");
    }

    auto testFmtOnUtf16Char() -> void
    {
        EXPECT_TRUE(fmt::format<char16_t>("Hello, World!"_sv) == u"Hello, World!");

        EXPECT_TRUE(fmt::format<char16_t>("Hello, World! {}!"_sv, 10) == u"Hello, World! 10!");

        EXPECT_TRUE(
            fmt::format<char16_t>("Hello, World! {}!"_sv, TestChar16StringView) ==
            u"Hello, "
            "World! Hello, World!!");

        EXPECT_TRUE(
            fmt::format<char16_t>("Hello, World! {}!"_sv, TestSetOfInts) ==
            u"Hello, World! {10, 20, 30, 40}!");

        EXPECT_TRUE(
            fmt::format<char16_t>("Hello, World! {}!"_sv, TestVectorOfInts) ==
            u"Hello, World! [10, 20, 30, 40]!");

        EXPECT_TRUE(
            fmt::format<char16_t>(
                "Hello, World! {} {} {} {}"_sv, 10, TestSetOfInts, TestVectorOfInts,
                TestCharStringView) ==
            u"Hello, World! 10 {10, 20, 30, 40} [10, 20, 30, 40] Hello, World!");
    }

    auto testFmt() -> int
    {
        testFmtOnBasicChar();
        testFmtOnUtf16Char();
        return 0;
    }
}// namespace cerb::debug
