#include <cerberus/debug/debug.hpp>
#include <cerberus/lex/generator_for_text.hpp>
#include <cerberus/range.hpp>

namespace cerb::debug
{
    using namespace lex;

    constexpr static std::string_view TestInput =
        "    \t\tHello, World! \nIt's a test "
        "\t\t  string.";

    auto testRawGeneratorForText() -> void
    {
        GeneratorForText<char> text_generator(TestInput, "None");

        for (size_t i : Range(TestInput.size())) {
            EXPECT_TRUE(text_generator.getRawChar() == TestInput[i]);
        }

        EXPECT_TRUE(isEoF(text_generator.getRawChar()));
        EXPECT_TRUE(isEoF(text_generator.getRawChar()));

        try {
            [[maybe_unused]] char _ = text_generator.getCurrentChar(1);
            CANT_BE_REACHED;
        } catch (TextGeneratorError const &) {
            MUST_BE_REACHED;
        }
    }

    auto testCleanGeneratorForText() -> void
    {
        constexpr static std::string_view expected_text = "Hello,World!It'sateststring.";
        constexpr static std::array<std::string_view, 28> expected_tabs_and_spaces = {
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

        GeneratorForText<char> text_generator(TestInput, "None");

        EXPECT_TRUE(isEoF(text_generator.getCurrentChar()));

        for (u32 i : Range(28U)) {
            char chr = text_generator.getCleanChar();
            EXPECT_TRUE(chr == expected_text[i]);
            EXPECT_TRUE(text_generator.getTabsAndSpaces() == expected_tabs_and_spaces[i]);
        }

        EXPECT_TRUE(isEoF(text_generator.getCleanChar()));
        EXPECT_TRUE(isEoF(text_generator.getCleanChar()));

        try {
            [[maybe_unused]] char _ = text_generator.getCurrentChar(1);
            CANT_BE_REACHED;
        } catch (TextGeneratorError const &) {
            MUST_BE_REACHED;
        }
    }

    auto testGeneratorForText() -> int
    {
        testRawGeneratorForText();
        testCleanGeneratorForText();
        return 0;
    }
}// namespace cerb::debug
