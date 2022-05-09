#include <cerberus/text/bracket_finder.hpp>

namespace cerb::text
{
    template class BracketFinder<char>;
    template class BracketFinder<char8_t>;
    template class BracketFinder<char16_t>;
}// namespace cerb::text
