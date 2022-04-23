#include <cerberus/debug/debug.hpp>
#include <cerberus/lex/item/regex_parser.hpp>

namespace cerb::debug
{
    static constexpr cerb::string_view TestInput = "[_0-9A-Za-z]";

    constexpr auto testRegexParserOnBasicString() -> bool
    {
        return true;
    }

    auto testRegexParser() -> int
    {
        return 0;
    }
}// namespace cerb::debug