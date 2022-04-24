#include <cerberus/lex/bracket_finder.hpp>

namespace cerb::lex
{
    template class BracketFinder<char>;
    template class BracketFinder<char8_t>;
    template class BracketFinder<char16_t>;
}// namespace cerb::lex
