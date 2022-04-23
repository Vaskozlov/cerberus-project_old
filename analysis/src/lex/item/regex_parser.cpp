#include <cerberus/lex/item/regex_parser.hpp>

namespace cerb::lex::regex
{
    template struct RegexParser<char>;
    template struct RegexParser<char8_t>;
    template struct RegexParser<char16_t>;
}// namespace cerb::lex::regex
