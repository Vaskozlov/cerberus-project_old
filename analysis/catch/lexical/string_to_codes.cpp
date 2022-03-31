#include <cerberus/debug/debug.hpp>
#include <cerberus/lex/string_to_codes.hpp>

namespace cerb::debug
{
    using namespace lex;
    using namespace std::string_view_literals;

    constexpr static std::string_view TestInput =
        "\"    \\t\\tHello, World! \\nIt's a test \\xFF \\0 \\077 \\0555 Привет мир!"
        "\\t\\t  string.\""sv;

    constexpr static std::string_view ExpectedOutput =
        "    \t\tHello, World! \nIt's a test \xFF \0 \077 \0555 Привет мир!"
        "\t\t  string."sv;

    constexpr static std::u16string_view TestInputU16 =
        u"\"    \\t\\tHello, World! \\nIt's a test \\xFF \\0 \\077 \\0555 Привет мир! \\uFFFF"
        "\\t\\t  string.\""sv;

    constexpr static std::u16string_view ExpectedOutputU16 =
        u"    \t\tHello, World! \nIt's a test \xFF \0 \077 \0555 Привет мир! \uFFFF"
        "\t\t  string."sv;

    auto testStringToCodesOnBasicString() -> bool
    {
        StringToCodes<char> string_to_codes{ '\"', TestInput };
        auto const &processed_string = string_to_codes.parseString();

        return processed_string == ExpectedOutput;
    }

    auto testStringToCodesOnUtf16String() -> bool
    {
        StringToCodes<char16_t> string_to_codes{ u'\"', TestInputU16 };
        auto const &processed_string = string_to_codes.parseString();

        return processed_string == ExpectedOutputU16;
    }

    auto testStringToCodes() -> int
    {
        EXPECT_TRUE(testStringToCodesOnBasicString());
        EXPECT_TRUE(testStringToCodesOnUtf16String());
        return 0;
    }
}// namespace cerb::debug