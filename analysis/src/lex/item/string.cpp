#include <cerberus/lex/item/string.hpp>

namespace cerb::lex::string
{
    template struct StringItem<char>;
    template struct StringItem<char16_t>;
}// namespace cerb::lex::string