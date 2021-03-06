#include <cerberus/debug/debug.hpp>
#include <cerberus/text/string_to_codes.hpp>

namespace cerb::debug
{
    using namespace lex;
    using namespace text;
    using namespace string_view_literals;

    constexpr static string_view TestInput =
        "\"    \\t\\tHello, World! \\nIt's a test \\xFF \\0 \\077 \\0555 Привет мир!"
        "\\t\\t  string.\""_sv;

    constexpr static string_view ExpectedOutput =
        "    \t\tHello, World! \nIt's a test \xFF \0 \077 \0555 Привет мир!"
        "\t\t  string."_sv;

    constexpr static u16string_view TestInputU16 =
        u"\"    \\t\\tHello, World! \\nIt's a test \\xFF \\0 \\077 \\0555 Привет мир! \\uFFFF"
        "\\t\\t  string.\""_sv;

    constexpr static u16string_view ExpectedOutputU16 =
        u"    \t\tHello, World! \nIt's a test \xFF \0 \077 \0555 Привет мир! \uFFFF"
        "\t\t  string."_sv;

    auto testStringToCodesOnEmptyBasicString() -> bool
    {
        GeneratorForText<char> text_generator{ "\"\"" };
        StringToCodes string_to_codes{ '\"', text_generator };
        auto const &parsed_string = string_to_codes.parsedString();

        return parsed_string.empty();
    }

    auto testStringToCodesOnBasicString() -> bool
    {
        GeneratorForText text_generator{ TestInput };
        StringToCodes string_to_codes{ '\"', text_generator };
        auto const &processed_string = string_to_codes.parsedString();

        return processed_string == ExpectedOutput;
    }

    auto testStringToCodesOnUtf16String() -> bool
    {
        GeneratorForText text_generator{ TestInputU16 };
        StringToCodes string_to_codes{ u'\"', text_generator };
        auto const &processed_string = string_to_codes.parsedString();

        return processed_string == ExpectedOutputU16;
    }

    auto testStringToCodes() -> int
    {
        ASSERT_TRUE(testStringToCodesOnEmptyBasicString());
        ASSERT_TRUE(testStringToCodesOnBasicString());
        ASSERT_TRUE(testStringToCodesOnUtf16String());
        return 0;
    }
}// namespace cerb::debug