#include <cerberus/debug/debug.hpp>
#include <cerberus/format/format.hpp>
#include <fmt/format.h>
#include <set>
#include <vector>

namespace cerb::debug
{
    constexpr static string_view TestCharStringView = "Hello, World!";
    constexpr static u16string_view TestChar16StringView = u"Hello, World!";
    const static std::set<int> TestSetOfInts = { 10, 20, 30, 40 };
    const static std::vector<int> TestVectorOfInts = { 10, 20, 30, 40 };

    auto testFmtOnBasicChar() -> void
    {
        EXPECT_TRUE(fmt::format<char>("Hello, World!") == "Hello, World!");

        EXPECT_TRUE(fmt::format<char>("Hello, World! {}!", 10) == "Hello, World! 10!");

        EXPECT_TRUE(
            fmt::format<char>("Hello, World! {}!", TestSetOfInts) ==
            "Hello, World! {10, 20, 30, 40}!");

        EXPECT_TRUE(
            fmt::format<char>("Hello, World! {}!", TestVectorOfInts) ==
            "Hello, World! [10, 20, 30, 40]!");

        EXPECT_TRUE(
            fmt::format<char>(
                "Hello, World! {} {} {} {}", 10, TestSetOfInts, TestVectorOfInts,
                TestCharStringView) ==
            "Hello, World! 10 {10, 20, 30, 40} [10, 20, 30, 40] Hello, World!");
    }

    auto testFmtOnUtf16Char() -> void
    {
        EXPECT_TRUE(fmt::format<char16_t>("Hello, World!") == u"Hello, World!");

        EXPECT_TRUE(fmt::format<char16_t>("Hello, World! {}!", 10) == u"Hello, World! 10!");

        EXPECT_TRUE(
            fmt::format<char16_t>("Hello, World! {}!", TestChar16StringView) ==
            u"Hello, "
            "World! Hello, World!!");

        EXPECT_TRUE(
            fmt::format<char16_t>("Hello, World! {}!", TestSetOfInts) ==
            u"Hello, World! {10, 20, 30, 40}!");

        EXPECT_TRUE(
            fmt::format<char16_t>("Hello, World! {}!", TestVectorOfInts) ==
            u"Hello, World! [10, 20, 30, 40]!");

        EXPECT_TRUE(
            fmt::format<char16_t>(
                "Hello, World! {} {} {} {}", 10, TestSetOfInts, TestVectorOfInts,
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
