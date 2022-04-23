#include <cerberus/lex/string_to_codes.hpp>

namespace cerb::lex
{
    template class StringToCodes<char>;
    template class StringToCodes<char16_t>;
}// namespace cerb::lex