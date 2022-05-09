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

    auto testRawGeneratorForText() -> void
    {
        GeneratorForText<char> text_generator(TestInput, "None");

        for (size_t i : Range(TestInput.size())) {
            EXPECT_TRUE(text_generator.getRawChar() == TestInput[i]);
        }

        EXPECT_TRUE(isEoF(text_generator.getRawChar()));
        EXPECT_TRUE(isEoF(text_generator.getRawChar()));

        EXPECT_TRUE(text_generator.getCurrentChar(1) == '\0');
    }

    auto testCleanGeneratorForText() -> void
    {
        constexpr static string_view expected_text = "Hello,World!It'sateststring.";
        constexpr static std::array<string_view, expected_text.size()> expected_tabs_and_spaces = {
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

        EXPECT_TRUE(isEoF(text_generator.getCurrentChar()));

        for (size_t i : Range(expected_text.size())) {
            char chr = text_generator.getCleanChar();
            EXPECT_TRUE(chr == expected_text[i]);
            EXPECT_TRUE(text_generator.getTabsAndSpaces() == expected_tabs_and_spaces[i]);
        }

        EXPECT_TRUE(isEoF(text_generator.getCleanChar()));
        EXPECT_TRUE(isEoF(text_generator.getCleanChar()));
        EXPECT_TRUE(text_generator.getCurrentChar(1) == '\0');
    }

    auto testGeneratorForText() -> int
    {
        testRawGeneratorForText();
        testCleanGeneratorForText();
        return 0;
    }
}// namespace cerb::debug
