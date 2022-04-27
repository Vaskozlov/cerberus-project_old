#include <cerberus/lex/item/item_parser.hpp>

namespace cerb::lex
{
    template struct ItemParser<char>;
    template struct ItemParser<char8_t>;
    template struct ItemParser<char16_t>;
}// namespace cerb::lex
