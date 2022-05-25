#include <cerberus/debug/debug.hpp>
#include <cerberus/range.hpp>
#include <cerberus/text/generator_for_text.hpp>

namespace cerb::debug
{
    using namespace lex;
    using namespace text;
    using namespace string_view_literals;

    constexpr static string_view TestInput =
        "    \t\tHello, World! \nIt's a test "
        "\t\t  string."_sv;

    CERBERUS_TEST_FUNC_STD_STRING(testRawGeneratorForText)
    {
        GeneratorForText<char> text_generator(TestInput, "None");

        for (size_t i : Range(TestInput.size())) {
            EXPECT_EQUAL(text_generator.getRawChar(), TestInput[i]);
        }

        EXPECT_EQUAL(text_generator.getRawChar(), '\0');
        EXPECT_EQUAL(text_generator.getRawChar(), '\0');
        EXPECT_EQUAL(text_generator.getCurrentChar(1), '\0');

        return true;
    }

    CERBERUS_TEST_FUNC_STD_STRING(testCleanGeneratorForText)
    {
        constexpr string_view expected_text = "Hello,World!It'sateststring.";
        constexpr std::array<std::string_view, expected_text.size()> expected_tabs_and_spaces = {
            "    \t\t",// H
            "",        // e
            "",        // l
            "",        // l
            "",        // o
            "",        // ,
            " ",       // W
            "",        // o
            "",        // r
            "",        // l
            "",        // d
            "",        // !
            "",        // I
            "",        // t
            "",        // '
            "",        // s
            " ",       // a
            " ",       // t
            "",        // e
            "",        // s
            "",        // t
            " \t\t  ", // s
            "",        // t
            "",        // r
            "",        // i
            "",        // n
            "",        // g
            "",        // t.
        };

        GeneratorForText<char> text_generator{ TestInput, "None" };

        EXPECT_EQUAL(text_generator.getCurrentChar(), '\0');

        for (size_t i : Range(expected_text.size())) {
            char chr = text_generator.getCleanChar();
            EXPECT_EQUAL(chr, expected_text[i]);
            EXPECT_EQUAL(text_generator.getTabsAndSpaces(), expected_tabs_and_spaces[i]);
        }

        EXPECT_EQUAL(text_generator.getCleanChar(), '\0');
        EXPECT_EQUAL(text_generator.getCleanChar(), '\0');
        EXPECT_EQUAL(text_generator.getCurrentChar(1), '\0');

        return true;
    }

    auto testGeneratorForText() -> int
    {
        CERBERUS_TEST_STD_STRING(testRawGeneratorForText());
        CERBERUS_TEST_STD_STRING(testCleanGeneratorForText());
        return 0;
    }
}// namespace cerb::debug
