#include <cerberus/lex/item/regex.hpp>

namespace cerb::lex::regex
{
    template struct RegexItem<char>;
    template struct RegexItem<char16_t>;
}// namespace cerb::lex::regex