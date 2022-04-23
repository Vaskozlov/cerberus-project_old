#include <cerberus/debug/debug.hpp>
#include <cerberus/lex/item/regex_parser.hpp>

namespace cerb::debug
{
    using namespace lex;
    using namespace regex;

    static constexpr cerb::string_view TestInput = "[_0-9A-Za-z]";

    auto testRegexParserOnBasicString() -> bool
    {
        ConstBitmap<1, 256> bitmap;
        GeneratorForText<char> text_generator{ TestInput };
        RegexParser<char> regex_parser{ text_generator, bitmap };

        return true;
    }

    auto testRegexParser() -> int
    {
        return 0;
    }
}// namespace cerb::debug
